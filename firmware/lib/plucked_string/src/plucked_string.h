#ifndef PLUCKED_STRING_H
#define PLUCKED_STRING_H

#include "Arduino.h"
#include "AudioStream.h"

// A plucked string, by the Karplus-Strong method: a burst of noise one period long
// circulates round a delay line as long as the string's period, losing a little
// each time round and more in the highs than the lows, which is how a string rings
// and mellows. The library's own AudioSynthKarplusStrong rounds the period to whole
// samples, so its high notes are out of tune, and can't move it while it rings;
// here the delay is read between samples, so the pitch is exact and can glide
// (vibrato, the bend, a retune).
//
// Input 0 moves the pitch, as AudioSynthWaveformModulated's frequency modulation
// input does (frequencyModulation() octaves at full scale); input 1 is passed
// through, so the string can sit after an oscillator and blend() crossfade from it
// (0, the oscillator alone, as if the string weren't there) to the string (1).
class AudioSynthPluckedString : public AudioStream
{
public:
  AudioSynthPluckedString() : AudioStream(2, inputQueueArray) {}
  void frequency(float freq) { base_freq = freq; }
  void frequencyModulation(float octaves) { fm_octaves = octaves; }
  void amplitude(float level) { amp = level; }
  void blend(float b) { mix = constrain(b, 0.0f, 1.0f); }
  // how long a note takes to die away by 60 dB, whatever its pitch
  void decay(float seconds) { t60 = max(seconds, 0.02f); tuned_freq = -1; }
  // 0-1: how much faster the highs die away than the lows
  void damping(float d) { damp = constrain(d, 0.0f, 1.0f) * 0.9f; tuned_freq = -1; }
  // 0-1: how bright the pluck is, from a muffled thump to a bright twang
  void pluck(float brightness = 1.0f);
  virtual void update(void);

private:
  audio_block_t *inputQueueArray[2];
  static const uint16_t length = 1024;   // the longest period: about 43 Hz
  float buffer[length] = {0};
  uint16_t write_index = 0;
  float base_freq = 220;
  float fm_octaves = 0;
  float amp = 0;
  float mix = 0;
  float t60 = 3;
  float damp = 0.27;
  float lowpass = 0;
  bool ringing = false;
  uint32_t seed = 22222;
  // for the pitch last tuned to
  float tuned_freq = -1;
  float delay_samples = 100;
  float loss = 1;
  void tune(float freq);
};

#endif
