/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off
enum layers{
    BASE,
    FN_1,
    FN_2,
    FN_3
};

enum custom_keycodes {
  LM_1 = SAFE_RANGE, // rainbow wave lighting mode
  LM_2, // gold static
  LM_3, // red static
  LM_T // RGB toggle
};
/*
    Description: List of tap dances

    * LIGHTS = Windows
*/
enum tap_dance_key_event {
    LIGHTS,
};

/*
    Description: Possible states for a given tap dance

    * SINGLE_HOLD   = 1: Activates while holding the key
    * SINGLE_TAP    = 2: Activates when pressing the key once
    * DOUBLE_TAP    = 3: Activates when pressing the key 2 times in quick succession
    * TRIPLE_TAP    = 4: Activates when pressing the key 3 times in quick succession
*/
enum tap_dance_state {
    SINGLE_HOLD = 1,
    SINGLE_TAP = 2,
    DOUBLE_TAP = 3,
    TRIPLE_TAP = 4,
};

/**
 * @brief Determines the state from tap dance state and converts to custom action
 * @returns Value of `tap_dance_state`
 */
int cur_dance(tap_dance_state_t *state) {
    if (state->pressed) {
        return SINGLE_HOLD;
    }
    if (state->count == 1) {
        return SINGLE_TAP;
    }
    if (state->count == 2) {
        return DOUBLE_TAP;
    }
    if (state->count == 3) {
        return TRIPLE_TAP;
    }

    return -1;
}

void lighting_td_finished(tap_dance_state_t *state, void *user_data) {
    lighting_td_state = cur_dance(state);
    switch (lighting_td_finished) {
        case DOUBLE_TAP:
            rgb_hsv = rgb_matrix_get_hsv();
            if (rgb_matrix_get_hue() == 36) {
                rgb_matrix_set_color_all(HSV_RED);
            } else if (rgb_matrix_get_hue() == 0) {
                rgb_matrix_set_color_all(HSV_GOLD);
            } else {
                rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
            }
            break;
        case SINGLE_HOLD:
                if (rgb_matrix_is_enabled()) {
                    rgb_matrix_disable();
                } else {
                    rgb_matrix_enable();
                }
            break;
        default:
            break;
    }
}

/*
    Description: Associate tap dance with defined functionality
*/
qk_tap_dance_action_t tap_dance_actions[] = {
    // Knob 0: Hold - Shift, One - (, Two - {, Three - [
    [LIGHTS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lighting_td_finished, NULL),
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(FN_1),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [FN_1] = LAYOUT_ansi_82(
        KC_TRNS,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,            RGB_TOG,
        KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  TD(LIGHTS),  KC_TRNS,            KC_TRNS,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
        KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_END,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

    [FN_2] = LAYOUT_ansi_82(
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,            KC_TRNS,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,   MO(FN_3),  KC_TRNS,  KC_TRNS,  KC_TRNS),

    [FN_3] = LAYOUT_ansi_82(
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            QK_BOOT,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            DB_TOGG,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,            EE_CLR ,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),
};
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN_1] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [FN_2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN_3] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}
};
#endif // ENCODER_MAP_ENABLE
bool dip_switch_update_user(uint8_t index, bool active) {
    if (active) {
      layer_clear();
      layer_on(FN_2);
    } else {
      layer_clear();
      layer_on(BASE);
    }

    return true;
}
