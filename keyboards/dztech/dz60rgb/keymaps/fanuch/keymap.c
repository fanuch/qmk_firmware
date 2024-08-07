#include QMK_KEYBOARD_H
// #include "tap_dance.h"

enum {
    TD_SCLN,
};

void dance_egg(tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
        tap_code(KC_BSLS);
    // } elif (state->count >= 2) {
    //     SEND_STRING("Safety dance!");
    //     reset_tap_dance(state);
    } else {
		tap_code(KC_SCRL);
		wait_ms(100);
		tap_code(KC_SCRL);
	}
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_SCLN] = ACTION_TAP_DANCE_FN(dance_egg),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Qwerty
 * ,----------------------------------------------------------------------------------------------------.
 * | `~     |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  -_  |  =+   | Bksp |
 * |--------+------+------+------+------+------+------+------+------+------+------+-------+------+------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  [{   |  ]}  |  \|  |
 * |--------+------+------+------+------+------+------+------+------+------+------+-------+------+------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  TD  |  "'   |    Enter    |
 * |--------+------+------+------+------+------+------+------+------+------+------+-------+------+------|
 * | Shift  |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |  ,<  |  .>  |  /? / Shift  |  Up  | Del  |
 * |--------+------+------+------+------+------+------+------+------+------+------+-------+------+------|
 * | Ctrl   | GUI  | Alt  |                  Space                  |  Alt |  MOD |  Left | Down | Right|
 * `----------------------------------------------------------------------------------------------------'
 */
	[0] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_SCRL, TD(TD_SCLN), CTL_T(KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, LT(1,KC_SCLN), LT(2,KC_QUOT), KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, RSFT_T(KC_SLSH), KC_UP, LT(2,KC_DEL), KC_LCTL, KC_LGUI, LM(1, MOD_LALT), KC_SPC, KC_RALT, MO(1), KC_LEFT, KC_DOWN, KC_RGHT),
/* Lower
 * ,-------------------------------------------------------------------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Btn2 |  Up  |PScrn | PgUp | Ins  |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Left | Down | Right| PgDn | Home |      |      |      |      |      |             |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      | VolU | Mute |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |                   Play                  |      |      | Prev | VolD | Next |
 * `-------------------------------------------------------------------------------------------------'
 */
	[1] = LAYOUT(KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_TRNS, KC_TRNS, KC_BTN2, KC_UP, KC_PSCR, KC_PGUP, KC_TRNS, KC_INS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_TRNS, KC_HOME, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, EE_CLR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_MUTE, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY, KC_TRNS, KC_TRNS, KC_MPRV, KC_VOLD, KC_MNXT),
/* Raise
 * ,-------------------------------------------------------------------------------------------------.
 * | NUM  |  /   |  *   |  -   |  +   |      |      |      |      |  /   |  *   |  -   |  +   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |  P7  |  P8  |  P9  | TOG  | MOD  | HUI  | SAI  | VAI  | SPI  |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Dot  |  P4  |  P5  |  P6  |      | RMOD | HUD  | SAD  | VAD  | SPD  |      |             |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|  P0  |  P1  |  P2  |  P3  |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |                   Space                 |      |      |      |      |      |
 * `-------------------------------------------------------------------------------------------------'
 */
	[2] = LAYOUT(KC_NUM, KC_PSLS, KC_PAST, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSLS, KC_PAST, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P7, KC_P8, KC_P9, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, KC_TRNS, KC_TRNS, TO(0), KC_TRNS, KC_PDOT, KC_P4, KC_P5, KC_P6, KC_TRNS, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, KC_TRNS, KC_PENT, KC_LSFT, KC_P0, KC_P1, KC_P2, KC_P3, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SPC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

