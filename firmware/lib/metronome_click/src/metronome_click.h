#ifndef METRONOME_CLICK_H
#define METRONOME_CLICK_H

#include "Arduino.h"
#include "AudioStream.h"

// A metronome's tick: a short sine that dies away in a few milliseconds, like a woodblock. It sends
// no block at all between ticks, so the mixer it feeds, and everything after it, can rest, where
// AudioSynthSimpleDrum fills a block of silence every 2.9 ms whether it sounds or not.
class AudioSynthClick : public AudioStream
{
public:
  AudioSynthClick() : AudioStream(0, NULL) {}
  // a tick at freq Hz, at amplitude 0-1; from the loop (the audio interrupt reads what it sets)
  void tick(float freq, float amplitude) {
    float w = 2.0f * (float)M_PI * freq / AUDIO_SAMPLE_RATE_EXACT;
    __disable_irq();
    k = 2.0f * cosf(w);   // sin(n w) by the recurrence y[n] = k y[n-1] - y[n-2]
    y1 = 0;
    y2 = -sinf(w);
    envelope = amplitude * 32000.0f;
    remaining = AUDIO_SAMPLE_RATE_EXACT * 0.04f;   // 40 ms, by when it has died to -60 dB
    __enable_irq();
  }
  virtual void update(void) {
    if (remaining <= 0) return;
    audio_block_t *block = allocate();
    if (!block) return;
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      float y = k * y1 - y2;
      y2 = y1;
      y1 = y;
      block->data[i] = (int16_t)(y * envelope);
      envelope *= decay;
    }
    remaining -= AUDIO_BLOCK_SAMPLES;
    transmit(block);
    release(block);
  }

private:
  float k = 0, y1 = 0, y2 = 0, envelope = 0;
  const float decay = 0.99637f;   // a 6 ms time constant at 44.1 kHz
  volatile int32_t remaining = 0;
};

#endif
