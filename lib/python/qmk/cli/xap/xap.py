"""Interactions with compatible XAP devices
"""
import cmd

from milc import cli

from qmk.keyboard import render_layout
from qmk.xap.common import get_xap_keycodes

from xap_client import XAPClient, XAPEventType, XAPSecureStatus

KEYCODE_MAP = get_xap_keycodes('latest')


def print_dotted_output(kb_info_json, prefix=''):
    """Print the info.json in a plain text format with dot-joined keys.
    """
    for key in sorted(kb_info_json):
        new_prefix = f'{prefix}.{key}' if prefix else key

        if key in ['parse_errors', 'parse_warnings']:
            continue
        elif key == 'layouts' and prefix == '':
            cli.echo('    {fg_blue}layouts{fg_reset}: %s', ', '.join(sorted(kb_info_json['layouts'].keys())))
        elif isinstance(kb_info_json[key], bytes):
            conv = "".join(["{:02X}".format(b) for b in kb_info_json[key]])
            cli.echo('    {fg_blue}%s{fg_reset}: %s', new_prefix, conv)
        elif isinstance(kb_info_json[key], dict):
            print_dotted_output(kb_info_json[key], new_prefix)
        elif isinstance(kb_info_json[key], list):
            data = kb_info_json[key]
            if len(data) and isinstance(data[0], dict):
                for index, item in enumerate(data, start=0):
                    cli.echo('    {fg_blue}%s.%s{fg_reset}: %s', new_prefix, index, str(item))
            else:
                cli.echo('    {fg_blue}%s{fg_reset}: %s', new_prefix, ', '.join(map(str, data)))
        else:
            cli.echo('    {fg_blue}%s{fg_reset}: %s', new_prefix, kb_info_json[key])


def _list_devices():
    """Dump out available devices
    """
    cli.log.info('Available devices:')
    devices = XAPClient.list()
    for dev in devices:
        device = XAPClient().connect(dev)

        data = device.info()
        cli.log.info('  %04x:%04x %s %s [API:%s]', dev['vendor_id'], dev['product_id'], dev['manufacturer_string'], dev['product_string'], data['_version']['xap'])

        if cli.config.general.verbose:
            # TODO: better formatting like 'lsusb -v'?
            print_dotted_output(data)


class XAPShell(cmd.Cmd):
    intro = 'Welcome to the XAP shell.  Type help or ? to list commands.\n'
    prompt = 'Ψ> '

    def __init__(self, device):
        cmd.Cmd.__init__(self)
        self.device = device
        # cache keycodes for this device
        self.keycodes = get_xap_keycodes(device.version()['xap'])

    def do_about(self, arg):
        """Prints out the version info of QMK
        """
        data = self.device.version()
        print_dotted_output(data)

    def do_status(self, arg):
        """Prints out the current device state
        """
        status = self.device.status()
        print('Secure:%s' % status.get('lock', '???'))

    def do_unlock(self, arg):
        """Initiate secure unlock
        """
        self.device.unlock()
        print('Unlock Requested...')

    def do_lock(self, arg):
        """Disable secure routes
        """
        self.device.lock()

    def do_reset(self, arg):
        """Jump to bootloader if unlocked
        """
        if not self.device.reset():
            print("Reboot to bootloader failed")
        return True

    def do_listen(self, arg):
        """Log out XAP broadcast messages
        """
        try:
            cli.log.info('Listening for XAP broadcasts...')
            while 1:
                (event, data) = self.device.listen()

                if event == XAPEventType.SECURE_STATUS:
                    secure_status = XAPSecureStatus(data[0]).name

                    cli.log.info('  Secure[%s]', secure_status)
                else:
                    cli.log.info('  Broadcast: type[%02x] data:[%s]', event, data.hex())

        except KeyboardInterrupt:
            cli.log.info('Stopping...')

    def do_keycode(self, arg):
        """Prints out the keycode value of a certain layer, row, and column
        """
        data = bytes(map(int, arg.split()))
        if len(data) != 3:
            cli.log.error('Invalid args')
            return

        keycode = self.device.transaction(b'\x04\x03', data)
        keycode = int.from_bytes(keycode, 'little')
        print(f'keycode:{self.keycodes.get(keycode, "unknown")}[{keycode}]')

    def do_keymap(self, arg):
        """Prints out the keycode values of a certain layer
        """
        data = bytes(map(int, arg.split()))
        if len(data) != 1:
            cli.log.error('Invalid args')
            return

        info = self.device.info()
        rows = info['matrix_size']['rows']
        cols = info['matrix_size']['cols']

        for r in range(rows):
            for c in range(cols):
                q = data + r.to_bytes(1, byteorder='little') + c.to_bytes(1, byteorder='little')
                keycode = self.device.transaction(b'\x04\x03', q)
                keycode = int.from_bytes(keycode, 'little')
                print(f'| {self.keycodes.get(keycode, "unknown").ljust(7)} ', end='', flush=True)
            print('|')

    def do_layer(self, arg):
        """Renders keycode values of a certain layer
        """
        data = bytes(map(int, arg.split()))
        if len(data) != 1:
            cli.log.error('Invalid args')
            return

        info = self.device.info()

        # Assumptions on selected layout rather than prompt
        first_layout = next(iter(info['layouts']))
        layout = info['layouts'][first_layout]['layout']

        keycodes = []
        for item in layout:
            q = data + bytes(item['matrix'])
            keycode = self.device.transaction(b'\x04\x03', q)
            keycode = int.from_bytes(keycode, 'little')
            keycodes.append(self.keycodes.get(keycode, '???'))

        print(render_layout(layout, False, keycodes))

    def do_exit(self, line):
        """Quit shell
        """
        return True

    def do_EOF(self, line):  # noqa: N802
        """Quit shell (ctrl+D)
        """
        return True

    def loop(self):
        """Wrapper for cmdloop that handles ctrl+C
        """
        try:
            self.cmdloop()
            print('')
        except KeyboardInterrupt:
            print('^C')
        return False


@cli.argument('-d', '--device', help='device to select - uses format <pid>:<vid>.')
@cli.argument('-l', '--list', arg_only=True, action='store_true', help='List available devices.')
@cli.argument('-i', '--interactive', arg_only=True, action='store_true', help='Start interactive shell.')
@cli.argument('action', nargs='*', arg_only=True, default=['listen'], help='Shell command and any arguments to run standalone')
@cli.subcommand('Acquire debugging information from usb XAP devices.', hidden=False if cli.config.user.developer else True)
def xap(cli):
    """Acquire debugging information from XAP devices
    """
    if cli.args.list:
        return _list_devices()

    # Connect to first available device
    devices = XAPClient.list()
    if not devices:
        cli.log.error('No devices found!')
        return False

    dev = devices[0]
    cli.log.info('Connecting to: %04x:%04x %s %s', dev['vendor_id'], dev['product_id'], dev['manufacturer_string'], dev['product_string'])
    device = XAPClient().connect(dev)

    # shell?
    if cli.args.interactive:
        XAPShell(device).loop()
        return True

    XAPShell(device).onecmd(' '.join(cli.args.action))
