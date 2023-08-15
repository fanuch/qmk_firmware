# QMK Firmware for my DZTech DZ60RGB

## Quick Steps
- Go to config.qmk.fm
- Upload `fanuch.json` if you have it
- if not, load default keymap for DZtech DZ60RBG or click this [link](https://config.qmk.fm/#/dztech/dz60rgb/v2_1/LAYOUT)
- Make your mods
- Download keymap.json
- Convert to keymap.c with
  - `qmk compile -kb dztech/dz60rgb/v2_1 -km fanuch`
- Set keyboard to flash mode with keybind:
  - `; + \`
- Keyboard will reappear as a flash device on:
  - `/media/$USER/KBDFANS`
- Flash with:
  - `sudo dd if=/Git/my_qmk_firmware/dztech_dz60rgb_v2_1_fanuch.bin  of=/media/$USER/KBDFANS/FLASH.BIN bs=512 conv=notrunc oflag=direct,sync`
- Unmount with:
  - `sudo unmount /media/$USER/KBDFANS`

## Detailed Instructions

### Set Config to the following
```
multibuild.keymap=default
user.keyboard=dztech/dz60rgb/v2_1
user.keymap=fanuch
user.qmk_home=~/Git/my_qmk_firmware
```

## How 
```
qmk json2c fanuch\(6\).json -o ~/Git/my_qmk_firmware/keyboards/dztech/dz60rgb/keymaps/fanuch/keymap.c
```

## Auto flashing

> This will automatically flash once the keyboard appears as storage device
```
while [ ! -d /media/$USER/KBDFANS ]; do sleep 1; done; sudo dd if=/home/$USER/Git/my_qmk_firmware/dztech_dz60rgb_v2_1_fanuch.bin of=/media/$USER/KBDFANS/FLASH.BIN bs=512 conv=notrunc oflag=direct,sync; sudo umount /media/$USER/KBDFANS
```