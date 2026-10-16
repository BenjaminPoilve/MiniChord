#ifndef LEAN_FILTER_H
#define LEAN_FILTER_H

#include "Arduino.h"
#include "AudioStream.h"

// The audio library's AudioFilterStateVariable (filter_variable.cpp, MIT licence), the same sums
// sample for sample, doing less of them:
// - with a control input, AudioFilterStateVariable works the corner frequency out afresh for every
//   sample (an exp2 approximation and more), though it only changes when the control does; a block
//   whose control holds still, as an envelope's does at sustain, takes it once for the block;
// - with lowpass_only(true) it works out and sends only the low pass (output 0), for a filter whose
//   band and high passes go nowhere.
class AudioFilterStateVariableLean : public AudioStream
{
public:
  AudioFilterStateVariableLean() : AudioStream(2, inputQueueArray) {
    frequency(1000);
    octaveControl(1.0);
    resonance(0.707);
  }
  void frequency(float freq) {
    if (freq < 20.0f) freq = 20.0f;
    else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2.5f) freq = AUDIO_SAMPLE_RATE_EXACT / 2.5f;
    setting_fcenter = (freq * (3.141592654f / (AUDIO_SAMPLE_RATE_EXACT * 2.0f))) * 2147483647.0f;
    setting_fmult = sinf(freq * (3.141592654f / (AUDIO_SAMPLE_RATE_EXACT * 2.0f))) * 2147483647.0f;
  }
  void resonance(float q) {
    if (q < 0.7f) q = 0.7f;
    else if (q > 5.0f) q = 5.0f;
    setting_damp = (1.0f / q) * 1073741824.0f;
  }
  void octaveControl(float n) {
    if (n < 0.0f) n = 0.0f;
    else if (n > 6.9999f) n = 6.9999f;
    setting_octavemult = n * 4096.0f;
  }
  void lowpass_only(bool on) { only_lowpass = on; }
  virtual void update(void);

private:
  template <bool all, bool variable>
  void run(const int16_t *in, const int16_t *ctl, int32_t fmult, int16_t *lp, int16_t *bp, int16_t *hp);
  int32_t setting_fcenter;
  int32_t setting_fmult;
  int32_t setting_octavemult;
  int32_t setting_damp;
  int32_t state_inputprev = 0;
  int32_t state_lowpass = 0;
  int32_t state_bandpass = 0;
  bool only_lowpass = false;
  audio_block_t *inputQueueArray[2];
};

#endif
