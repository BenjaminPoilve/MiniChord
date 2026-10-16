#ifndef SILENCE_GATE_H
#define SILENCE_GATE_H

#include "Arduino.h"
#include "AudioStream.h"

// Passes a block on only if something in it is louder than threshold(), and otherwise passes
// nothing. The audio library only works on objects that receive blocks, and an object sends no
// block when it gets none, so a path that has gone quiet can go idle. Not every path does by
// itself: a delay's feedback loop hands its blocks round for good once one has entered (the mixer
// passes any block on, the delay re-sends what it queued, the filter answers every block), however
// silent they have become, and keeps everything after it running. A gate in the loop ends that.
class AudioEffectSilenceGate : public AudioStream
{
public:
  AudioEffectSilenceGate() : AudioStream(1, inputQueueArray) {}
  void threshold(int16_t level) { limit = level; }
  virtual void update(void) {
    audio_block_t *block = receiveReadOnly(0);
    if (!block) return;
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      if (block->data[i] > limit || block->data[i] < -limit) {
        transmit(block);
        break;
      }
    }
    release(block);
  }

private:
  audio_block_t *inputQueueArray[1];
  int16_t limit = 4;   // 4 LSB, -78 dBFS: below anything heard, above the filters' rounding residue
};

#endif
