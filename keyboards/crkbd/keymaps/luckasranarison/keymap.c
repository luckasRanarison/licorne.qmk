/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "raw_hid.h"

#define HID_MSG_LEN 32

const char *layers[] PROGMEM = {
    "BASE",
    "SYMBOLS",
    "OPERATORS",
    "MOUSE",
    "EDIT",
    "FUNCTIONS",
    "SECRETS",
};

enum custom_keycodes {
    KC_SARW = SAFE_RANGE, // ->
    KC_FARW,              // =>
    KC_DEQ,               // ==
    KC_TEQ,               // ===
    KC_SNEQ,              // !=
    KC_FNEQ,              // !==
    KC_HEQ,               // >=
    KC_LEQ,               // =<
    KC_AND,               // &&
    KC_OR,                // ||
    KC_NULL,              // ??
    KC_DOTS,              // ...
    KC_SCPE,              // ::
    KC_FISH,              // ::<_>()
    KC_MSE,               // Mouse layer toggle
};

const char *custom_keys[] PROGMEM = {
    "->",
    "=>",
    "==",
    "===",
    "!=",
    "!==",
    ">=",
    "<=",
    "&&",
    "||",
    "??",
    "...",
    "::",
    "::<_>()",
};

#define TD_ENSH RSFT_T(KC_ENT)
#define TD_MSFN LT(4, KC_MSE)
#define TD_ZERO LT(2, KC_0)
#define KC_INF LSFT(KC_COMM)
#define KC_SUP LSFT(KC_DOT)

enum combos {
    CB_LGUI,
    CB_LSFT,
    CB_LCTL,
    CB_LALT,
    CB_RALT,
    CB_RCTL,
    CB_RSFT,
    CB_RGUI,
    CB_OPER,
    CB_CAPS,
};

const uint16_t PROGMEM cb_lgui[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM cb_lsft[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM cb_lctl[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM cb_lalt[] = {KC_S, KC_F, COMBO_END};
const uint16_t PROGMEM cb_ralt[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM cb_rctl[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM cb_rsft[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM cb_rgui[] = {KC_L, KC_SCLN, COMBO_END};
const uint16_t PROGMEM cb_oper[] = {TD_MSFN, MO(1), COMBO_END};
const uint16_t PROGMEM cb_caps[] = {TD_ENSH, KC_SPACE, COMBO_END};

combo_t key_combos[] = {
    [CB_LGUI] = COMBO(cb_lgui, KC_LGUI),
    [CB_LSFT] = COMBO(cb_lsft, KC_LSFT),
    [CB_LCTL] = COMBO(cb_lctl, KC_LCTL),
    [CB_LALT] = COMBO(cb_lalt, KC_LALT),
    [CB_RALT] = COMBO(cb_ralt, KC_RALT),
    [CB_RCTL] = COMBO(cb_rctl, KC_RCTL),
    [CB_RSFT] = COMBO(cb_rsft, KC_RSFT),
    [CB_RGUI] = COMBO(cb_rgui, KC_RGUI),
    [CB_OPER] = COMBO(cb_oper, MO(2)),
    [CB_CAPS] = COMBO(cb_caps, KC_CAPS),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3_ex2( //                               BASE
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
       KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, RM_NEXT,    XXXXXXX,    KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, RM_PREV,    XXXXXXX,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            MO(5),   MO(1),  KC_SPC,    TD_ENSH, TD_MSFN,  OSL(6)
                                      //`--------------------------'  `--------------------------'
  ),

    [1] = LAYOUT_split_3x6_3_ex2( //                          SYMBOL/NUMBER
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      _______, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, KC_PIPE, XXXXXXX,    XXXXXXX, KC_PLUS,    KC_1,    KC_2,    KC_3, KC_MINS, _______,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      _______, KC_ASTR, KC_SLSH, KC_LPRN, KC_RPRN, KC_BSLS, XXXXXXX,    XXXXXXX,  KC_EQL,    KC_4,    KC_5,    KC_6, KC_UNDS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      _______, KC_EXLM, KC_TILD, KC_HASH, KC_PERC,   KC_AT,                      KC_AMPR,    KC_7,    KC_8,    KC_9,  KC_DLR, KC_CIRC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, _______, _______,    _______, TD_ZERO, XXXXXXX
                                      //`--------------------------'  `--------------------------'

  ),

    [2] = LAYOUT_split_3x6_3_ex2( //                             OPERATORS
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, KC_SCPE, KC_FNEQ, XXXXXXX, XXXXXXX,    XXXXXXX,  KC_TEQ,  KC_LEQ,  KC_HEQ, KC_SARW, KC_FISH, _______,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      _______,  KC_AND,   KC_OR, KC_NULL, KC_SNEQ, XXXXXXX, XXXXXXX,    XXXXXXX,  KC_DEQ,  KC_INF,  KC_SUP, KC_FARW, KC_DOTS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, _______, _______,    _______, XXXXXXX, _______
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3_ex2( //                              MOUSE
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, XXXXXXX, MS_WHLU, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, MS_WHLL, MS_BTN2, MS_BTN1, MS_WHLR, XXXXXXX,    XXXXXXX, MS_LEFT, MS_DOWN,   MS_UP, MS_RGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MS_WHLD, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  KC_CUT, KC_COPY,    KC_PSTE,   TG(3), XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

    [4] = LAYOUT_split_3x6_3_ex2( //                               EDIT
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_NEXT,    XXXXXXX,  KC_INS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX,  KC_CUT, KC_PSTE, KC_COPY, XXXXXXX, RM_PREV,    XXXXXXX, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,                       KC_DEL, KC_RCTL, KC_RSFT, KC_RALT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, XXXXXXX, XXXXXXX,    XXXXXXX, _______, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

    [5] = LAYOUT_split_3x6_3_ex2( //                             FUNCTION
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR, KC_MUTE, KC_VOLU,    KC_BRIU,  KC_PWR,   KC_F1,   KC_F2,   KC_F3,  KC_F10, KC_HOME,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, KC_MPRV, KC_MNXT, KC_VOLD,    KC_BRID, KC_SLEP,   KC_F4,   KC_F5,   KC_F6,  KC_F11, KC_PGUP,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      RM_NEXT, RM_HUED, RM_SATD, RM_VALD, KC_MSTP, KC_MPLY,                      KC_WAKE,   KC_F7,   KC_F8,   KC_F9,  KC_F12, KC_PGDN,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, KC_LALT,  KC_ENT,    KC_RCTL, KC_RSFT, KC_RGUI
                                      //`--------------------------'  `--------------------------'
  ),

    [6] = LAYOUT_split_3x6_3_ex2( //                           MACRO/SECRETS
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX,    MC_0, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    MC_2, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         MC_1, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  )
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    char *key;

    switch (keycode) {
    case TD_MSFN:
        if (record->tap.count && record->event.pressed) {
            layer_invert(3);
            return false;
        }
        break;
    case KC_SARW ... KC_FISH:
        if (record->event.pressed) {
            key = pgm_read_ptr(&custom_keys[keycode - SAFE_RANGE]);
            SEND_STRING(key);
            return false;
        }
        break;
    }

    // TODO: Implement macros with a secret system

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    char msg[HID_MSG_LEN] = "msg:layer:";
    uint8_t length = strlen(msg);
    uint8_t layer_index = get_highest_layer(state);
    char *layer_name = pgm_read_ptr(&layers[layer_index]);

    snprintf(msg + length, sizeof(msg) - length, "%s", layer_name);
    raw_hid_send((uint8_t *)msg, HID_MSG_LEN);

    return state;
}
