#include "lean_mixer.h"
#include "utility/dspinst.h"

// AudioMixer4's own gain and add, for Teensy 4 (mixer.cpp), so the sums come out the same
static void apply_gain(int16_t *dst, const int16_t *src, int32_t mult) {
  uint32_t *d = (uint32_t *)dst;
  const uint32_t *s = (const uint32_t *)src;
  const uint32_t *end = (uint32_t *)(dst + AUDIO_BLOCK_SAMPLES);
  do {
    uint32_t tmp32 = *s++;
    int32_t val1 = signed_multiply_32x16b(mult, tmp32);
    int32_t val2 = signed_multiply_32x16t(mult, tmp32);
    val1 = signed_saturate_rshift(val1, 16, 0);
    val2 = signed_saturate_rshift(val2, 16, 0);
    *d++ = pack_16b_16b(val2, val1);
  } while (d < end);
}

static void apply_gain_then_add(int16_t *data, const int16_t *in, int32_t mult) {
  uint32_t *dst = (uint32_t *)data;
  const uint32_t *src = (const uint32_t *)in;
  const uint32_t *end = (uint32_t *)(data + AUDIO_BLOCK_SAMPLES);
  if (mult == 65536) {
    do {
      uint32_t tmp32 = *dst;
      *dst++ = signed_add_16_and_16(tmp32, *src++);
      tmp32 = *dst;
      *dst++ = signed_add_16_and_16(tmp32, *src++);
    } while (dst < end);
  } else {
    do {
      uint32_t tmp32 = *src++;
      int32_t val1 = signed_multiply_32x16b(mult, tmp32);
      int32_t val2 = signed_multiply_32x16t(mult, tmp32);
      val1 = signed_saturate_rshift(val1, 16, 0);
      val2 = signed_saturate_rshift(val2, 16, 0);
      tmp32 = pack_16b_16b(val2, val1);
      uint32_t tmp32b = *dst;
      *dst++ = signed_add_16_and_16(tmp32, tmp32b);
    } while (dst < end);
  }
}

void AudioMixer4Lean::update(void) {
  audio_block_t *in[4];
  bool any = false;
  int live = 0, first = -1;
  for (int ch = 0; ch < 4; ch++) {
    in[ch] = receiveReadOnly(ch);
    if (!in[ch]) continue;
    any = true;
    if (multiplier[ch] == 0) {   // at gain 0 it would add nothing
      release(in[ch]);
      in[ch] = NULL;
      continue;
    }
    if (first < 0) first = ch;
    live++;
  }
  if (live == 0) {
    if (any && !quiet_when_muted) {   // AudioMixer4 sends a silent block for inputs all at 0
      audio_block_t *out = allocate();
      if (out) {
        memset(out->data, 0, sizeof(out->data));
        transmit(out);
        release(out);
      }
    }
    return;
  }
  if (live == 1 && multiplier[first] == unity) {   // one input at 1: on as it is
    transmit(in[first]);
    release(in[first]);
    return;
  }
  audio_block_t *out = allocate();
  if (out) {
    if (multiplier[first] == unity) memcpy(out->data, in[first]->data, sizeof(out->data));
    else apply_gain(out->data, in[first]->data, multiplier[first]);
  }
  release(in[first]);
  for (int ch = first + 1; ch < 4; ch++) {
    if (!in[ch]) continue;
    if (out) apply_gain_then_add(out->data, in[ch]->data, multiplier[ch]);
    release(in[ch]);
  }
  if (out) {
    transmit(out);
    release(out);
  }
}
