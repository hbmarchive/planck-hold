/* Copyright 2015-2023 Jack Humbert
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

enum my_layers {
  BASE_LAYER,
  LSYM_LAYER,
  RSYM_LAYER,
  NUM_LAYER,
  NAV_LAYER,
  CTRL_LAYER,
  SCUT_LAYER
};

enum my_keycodes {
    M_ESCQ = SAFE_RANGE,
    M_ESCW,
    M_ESCV,
    M_DDS,
    M_CSPC,
    M_DSC,
    M_ALTT,
    M_APP1,
    M_APP2,
    M_APP3,
    M_APP4,
    M_1PASS,
    M_NDESK,
    M_PDESK,
    M_XTAB,
    M_WMIN,
    M_WMAX,
    M_NTRM,
    M_EMOJI,
    M_ETCTLZ,
    M_ISCB,
    M_ISWIN
};

// Stores state of M_ALTT macro - true if we are currently tabbing between
// windows.
static bool m_altt_pressed = false;

// Toggle for keys that affect the the desktop - value can be changed in
// function layer
static bool m_is_chromebook = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE_LAYER] = LAYOUT_planck_grid(
    KC_ESC,          KC_Q,             KC_W,           KC_F,           KC_P,           KC_B,    KC_J,    KC_L,    KC_U,           KC_Y,           KC_NUBS,          KC_DEL,
    KC_TAB,          KC_A,             KC_R,           KC_S,           KC_T,           KC_G,    KC_M,    KC_N,    KC_E,           KC_I,           KC_O,             KC_BSPC,
    TO(CTRL_LAYER), OSL(RSYM_LAYER),   KC_X,           KC_C,           KC_D,           KC_V,    KC_K,    KC_H,    KC_COMM,        KC_DOT,         OSL(LSYM_LAYER), OSL(SCUT_LAYER),
    OSM(MOD_LCTL),   OSM(MOD_LALT),    OSM(MOD_LGUI),  MO(NAV_LAYER),  OSM(MOD_LSFT),  KC_SPC,  KC_ENT,  KC_ESC,  MO(NUM_LAYER),  OSM(MOD_RGUI),  OSM(MOD_RALT),    OSM(MOD_RCTL)
  ),

  [LSYM_LAYER] = LAYOUT_planck_grid(
    KC_TRNS,  KC_EXLM,  LSFT(KC_2),     LSFT(KC_3),  KC_DLR,   KC_PERC,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_TRNS,
    KC_TRNS,  KC_NO,    LSFT(KC_NUBS),  KC_LBRC,     KC_LCBR,  KC_LPRN,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_TRNS,
    KC_TRNS,  KC_NO,    KC_NUBS,        KC_RBRC,     KC_RCBR,  KC_RPRN,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_SLSH,  KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,        KC_TRNS,     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS
  ),

  [RSYM_LAYER] = LAYOUT_planck_grid(
    KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_CIRC,  KC_AMPR,        KC_ASTR,        KC_UNDS,  KC_PLUS,  KC_TRNS,
    KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_COLN,  LSFT(KC_QUOT),  LSFT(KC_BSLS),  KC_MINS,  KC_EQL,   KC_TRNS,
    KC_TRNS,  KC_Z,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_SCLN,  KC_QUOT,        KC_BSLS,        KC_GRV,   KC_NO,    KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,        KC_TRNS,        KC_TRNS,  KC_TRNS,  KC_TRNS
  ),

  [NUM_LAYER] = LAYOUT_planck_grid(
    KC_TRNS,  KC_PAST,  KC_1,     KC_2,     KC_3,     KC_PPLS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_TRNS,
    KC_TRNS,  KC_PSLS,  KC_4,     KC_5,     KC_6,     KC_PMNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_TRNS,
    KC_TRNS,  KC_DOT,   KC_7,     KC_8,     KC_9,     KC_0,     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS
  ),

  [NAV_LAYER] = LAYOUT_planck_grid(
    KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    M_XTAB,   M_PDESK,  LCTL(KC_TAB),  M_ALTT,   M_NDESK,  KC_TRNS,
    KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_WH_U,  KC_LEFT,  KC_DOWN,       KC_UP,    KC_RGHT,  KC_TRNS,
    KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_WH_D,  KC_HOME,  KC_PGDN,       KC_PGUP,  KC_END,   KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,       KC_TRNS,  KC_TRNS,  KC_TRNS
  ),

  [CTRL_LAYER] = LAYOUT_planck_grid(
    TO(BASE_LAYER),  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    M_ISWIN,         M_ISCB,   KC_PSCR,  KC_INS,        KC_TRNS,
    KC_TRNS,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_BRIU,  KC_VOLU,         KC_MNXT,  KC_MPLY,  LSFT(KC_INS),  KC_TRNS,
    KC_TRNS,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_BRID,  KC_VOLD,         KC_MPRV,  KC_MUTE,  LCTL(KC_INS),  KC_TRNS,
    KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  TO(BASE_LAYER),  KC_TRNS,  KC_TRNS,  KC_TRNS,       KC_TRNS


  ),

  [SCUT_LAYER]  = LAYOUT_planck_grid(
    KC_TRNS,  M_ESCQ,   M_ESCW,      LCTL(KC_F),  KC_NO,             LCTL(KC_B),  M_WMAX,      KC_NO,    KC_NO,    KC_NO,     KC_NO,    KC_TRNS,
    KC_TRNS,  M_APP1,   M_APP2,      M_APP3,      M_1PASS,           M_APP4,      M_WMIN,      M_NTRM,   M_EMOJI,  M_ETCTLZ,  KC_NO,    KC_TRNS,
    KC_CAPS,  KC_NO,    LCTL(KC_X),  LCTL(KC_C),  LSFT(LCTL(KC_C)),  LCTL(KC_V),  HYPR(KC_K),  M_DDS,    M_CSPC,   M_DSC,     KC_NO,  KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_TRNS,     KC_CAPS,           KC_TRNS,     KC_TRNS,     KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Stop pressing the alt key once a key other than the alt-tab macro has been
  // pressed.
  if (keycode != M_ALTT && m_altt_pressed) {
    unregister_code(KC_LALT);
    m_altt_pressed = false;
  }
  // Ensure shift is not pressed when the symbol layer is active.
  if (IS_LAYER_ON(LSYM_LAYER) || IS_LAYER_ON(RSYM_LAYER)) {
    // Allow the Z keycode in the LSYM layer to be capitalised.
    if (keycode != OSM(LSYM_LAYER) && keycode != KC_Z) {
      del_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);
    }
  }
  switch (keycode) {
    case M_ALTT:
      if (record->event.pressed) {
        if (!m_altt_pressed) {
          register_code(KC_LALT);
          m_altt_pressed = true;
        }
        tap_code(KC_TAB);
      }
      break;
    case M_APP1:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_1));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_1));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_APP2:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_2));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_2));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_APP3:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_3));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_3));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_APP4:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_4));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_4));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_1PASS:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT)SS_TAP(X_1)SS_UP(X_LALT));
          SEND_STRING(SS_DELAY(100));
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_X));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_SPC));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_ESCQ:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC)":q!"SS_TAP(X_ENT));
      } else {
        layer_move(BASE_LAYER);
      }
      break;
    case M_ESCW:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC)":wq"SS_TAP(X_ENT));
      } else {
        layer_move(BASE_LAYER);
      }
      break;
    case M_DDS:
      if (record->event.pressed) {
        SEND_STRING("../");
      } else {
        layer_move(BASE_LAYER);
      }
      break;
    case M_CSPC:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_COMM)SS_TAP(X_SPC));
      }
      break;
    case M_DSC:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_DOT)SS_TAP(X_SPC));
        add_oneshot_mods(MOD_LSFT);
      }
      break;
    case M_ESCV:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC));
        SEND_STRING(SS_DOWN(X_LSFT)SS_TAP(X_V)SS_UP(X_LSFT));
      }
      break;
    case M_NDESK:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_RBRC));
          SEND_STRING(SS_UP(X_LGUI));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_RGHT));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
        }
      }
      break;
    case M_PDESK:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_LBRC));
          SEND_STRING(SS_UP(X_LGUI));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_LEFT));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
        }
      }
      break;
    case M_XTAB:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_W)SS_UP(X_LCTL));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_F4)SS_UP(X_LCTL));
        }
      }
      break;
    case M_WMIN:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_MINS));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_M));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_WMAX:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_EQL));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_J));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_NTRM:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_T)SS_UP(X_LCTL));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_1));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_EMOJI:
      if (record->event.pressed) {
        if (m_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_SPC));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LSFT));
        } else {
          SEND_STRING(SS_DOWN(X_LGUI)SS_TAP(X_SCLN)SS_UP(X_LGUI));
        }
      }
      break;
    case M_ETCTLZ:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ENT));
        SEND_STRING(SS_DOWN(X_LSFT)SS_TAP(X_BSLS)SS_UP(X_LSFT));
        SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_Z)SS_UP(X_LCTL));
      }
      break;
    case M_ISCB:
      if (record->event.pressed) {
        m_is_chromebook = true;
      }
      break;
    case M_ISWIN:
      if (record->event.pressed) {
        m_is_chromebook = false;
      }
      break;
  }
  return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Return to the base layer if space, enter, home, end or a function key is
    // pressed.
    case KC_SPC:
    case KC_TAB:
    case KC_ENT:
    case KC_PSCR:
    case KC_INS:
    case LSFT(KC_INS):
    case LCTL(KC_INS):
    case KC_F1 ... KC_F12:
    case M_ISCB:
    case M_ISWIN:
      if (!record->event.pressed) { layer_move(BASE_LAYER); }
      break;
    // Cancel caps lock if escape is pressed.
    case KC_ESC:
      if (host_keyboard_led_state().caps_lock) { tap_code(KC_CAPS); }
      if (!record->event.pressed) { layer_move(BASE_LAYER); }
      break;
  }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Set the tapping term for the homerow mods.
    case LSFT_T(KC_Q):
    case LCTL_T(KC_W):
    case LALT_T(KC_F):
    case LGUI_T(KC_P):
    case LGUI_T(KC_L):
    case LALT_T(KC_U):
    case LCTL_T(KC_Y):
    case LSFT_T(KC_NUBS):
      return TAPPING_TERM_MODS;
    default:
      return TAPPING_TERM;
  }
}
