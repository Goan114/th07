// Generated from purple THPrac (MIT), TH07 source sha256 b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0.
#pragma once
#include <cstdint>
namespace PracticeRuntime {
struct PurpleInput {
 bool disable_xkey=false,disable_Ckey_at_same_time=true,disable_shiftkey=false,force_shiftkey=false,disable_zkey=false;
 void apply(std::uint8_t* state) const {
    if (disable_xkey) {
        state[88] = 0x0;
    }
    if (disable_xkey && disable_Ckey_at_same_time) {
        state[67] = 0x0;
    }
    if (disable_shiftkey) {
        state[160] = 0x0;
        state[161] = 0x0;
    }
    if (force_shiftkey)
    {
        state[160] = 0x80;
        state[161] = 0x80;
    }
    if (disable_zkey) {
        state[90] = 0x0;
    }
 }
 std::uint16_t filter(std::uint16_t buttons, bool enabled, bool replay) const {
  if (!enabled || replay) return buttons;
  std::uint8_t state[256]{}; state[90]=(buttons&1)?0x80:0;state[88]=(buttons&2)?0x80:0;
  state[160]=state[161]=(buttons&4)?0x80:0; apply(state);
  return (buttons & ~std::uint16_t(7)) | (state[90]?1:0) | (state[88]?2:0) | ((state[160]||state[161])?4:0);
 }
};
}
