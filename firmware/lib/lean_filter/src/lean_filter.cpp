#include "lean_filter.h"
#include "utility/dspinst.h"

// as filter_variable.cpp has it (Chamberlin, 2X oversampled; neither of its accuracy options on)
#define MULT(a, b) (multiply_32x32_rshift32_rounded(a, b) << 2)

// the corner for one control value: filter_variable.cpp's own sums (Laurent de Soras' exp2)
static inline int32_t fmult_for(int32_t control, int32_t fcenter, int32_t octavemult) {
  control *= octavemult;
  int32_t n = control & 0x7FFFFFF;
  n = (n + 134217728) << 3;
  n = multiply_32x32_rshift32_rounded(n, n);
  n = multiply_32x32_rshift32_rounded(n, 715827883) << 3;
  n = n + 715827882;
  n = n >> (6 - (control >> 27));
  int32_t fmult = multiply_32x32_rshift32_rounded(fcenter, n);
  if (fmult > 5378279) fmult = 5378279;
  return fmult << 8;
}

template <bool all, bool variable>
void AudioFilterStateVariableLean::run(const int16_t *in, const int16_t *ctl, int32_t fmult,
                                       int16_t *lp, int16_t *bp, int16_t *hp) {
  const int16_t *end = in + AUDIO_BLOCK_SAMPLES;
  const int32_t fcenter = setting_fcenter, octavemult = setting_octavemult, damp = setting_damp;
  int32_t inputprev = state_inputprev, lowpass = state_lowpass, bandpass = state_bandpass;
  do {
    if (variable) fmult = fmult_for(*ctl++, fcenter, octavemult);
    int32_t input = (*in++) << 12;
    lowpass = lowpass + MULT(fmult, bandpass);
    int32_t highpass = ((input + inputprev) >> 1) - lowpass - MULT(damp, bandpass);
    inputprev = input;
    bandpass = bandpass + MULT(fmult, highpass);
    int32_t lowpasstmp = lowpass, bandpasstmp = bandpass, highpasstmp = highpass;
    lowpass = lowpass + MULT(fmult, bandpass);
    highpass = input - lowpass - MULT(damp, bandpass);
    bandpass = bandpass + MULT(fmult, highpass);
    *lp++ = signed_saturate_rshift(lowpass + lowpasstmp, 16, 13);
    if (all) {
      *bp++ = signed_saturate_rshift(bandpass + bandpasstmp, 16, 13);
      *hp++ = signed_saturate_rshift(highpass + highpasstmp, 16, 13);
    }
  } while (in < end);
  state_inputprev = inputprev;
  state_lowpass = lowpass;
  state_bandpass = bandpass;
}

void AudioFilterStateVariableLean::update(void) {
  audio_block_t *input_block = receiveReadOnly(0);
  audio_block_t *control_block = receiveReadOnly(1);
  if (!input_block) {
    if (control_block) release(control_block);
    return;
  }
  audio_block_t *lowpass_block = allocate(), *bandpass_block = NULL, *highpass_block = NULL;
  bool all = !only_lowpass;
  if (lowpass_block && all) {
    bandpass_block = allocate();
    highpass_block = allocate();
  }
  if (!lowpass_block || (all && (!bandpass_block || !highpass_block))) {
    release(input_block);
    if (control_block) release(control_block);
    if (lowpass_block) release(lowpass_block);
    if (bandpass_block) release(bandpass_block);
    if (highpass_block) release(highpass_block);
    return;
  }
  int16_t *bp = all ? bandpass_block->data : NULL, *hp = all ? highpass_block->data : NULL;
  if (control_block) {
    const int16_t *ctl = control_block->data;
    bool flat = true;   // a control that holds still: its corner once, for the block
    for (int i = 1; i < AUDIO_BLOCK_SAMPLES && flat; i++) flat = ctl[i] == ctl[0];
    if (flat) {
      int32_t fmult = fmult_for(ctl[0], setting_fcenter, setting_octavemult);
      if (all) run<true, false>(input_block->data, NULL, fmult, lowpass_block->data, bp, hp);
      else run<false, false>(input_block->data, NULL, fmult, lowpass_block->data, bp, hp);
    } else {
      if (all) run<true, true>(input_block->data, ctl, 0, lowpass_block->data, bp, hp);
      else run<false, true>(input_block->data, ctl, 0, lowpass_block->data, bp, hp);
    }
    release(control_block);
  } else {
    if (all) run<true, false>(input_block->data, NULL, setting_fmult, lowpass_block->data, bp, hp);
    else run<false, false>(input_block->data, NULL, setting_fmult, lowpass_block->data, bp, hp);
  }
  release(input_block);
  transmit(lowpass_block, 0);
  release(lowpass_block);
  if (all) {
    transmit(bandpass_block, 1);
    release(bandpass_block);
    transmit(highpass_block, 2);
    release(highpass_block);
  }
}
