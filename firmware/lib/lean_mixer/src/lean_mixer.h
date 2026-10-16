#ifndef LEAN_MIXER_H
#define LEAN_MIXER_H

#include "Arduino.h"
#include "AudioStream.h"

// The audio library's AudioMixer4, with the same arithmetic sample for sample, doing less of it:
// - an input at gain 0 is let go unread, where AudioMixer4 multiplies all 128 samples by 0 and adds
//   them in;
// - one input left, at gain 1, is passed on as it is, where AudioMixer4 copies it (when another
//   object shares it) and adds nothing to it;
// - with every input there at gain 0 it sends a silent block, as AudioMixer4 does, or, with
//   silent_when_muted(true), nothing at all, so what follows it can go idle too. Only for a mixer
//   nothing after it needs blocks from: an envelope only moves through its stages while they come.
class AudioMixer4Lean : public AudioStream
{
public:
  AudioMixer4Lean(void) : AudioStream(4, inputQueueArray) {
    for (int i = 0; i < 4; i++) multiplier[i] = unity;
  }
  virtual void update(void);
  void gain(unsigned int channel, float gain) {
    if (channel >= 4) return;
    if (gain > 32767.0f) gain = 32767.0f;
    else if (gain < -32767.0f) gain = -32767.0f;
    multiplier[channel] = gain * 65536.0f;   // as AudioMixer4 rounds it
  }
  void silent_when_muted(bool on) { quiet_when_muted = on; }

private:
  static const int32_t unity = 65536;
  int32_t multiplier[4];
  bool quiet_when_muted = false;
  audio_block_t *inputQueueArray[4];
};

#endif
