/*  Stereo plate reverb for Teensy 4
 *
 *  Author: Piotr Zapart
 *          www.hexefx.com
 *
 * Copyright (c) 2020 by Piotr Zapart
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <Arduino.h>
#include "effect_platervbstereo.h"

#define INP_ALLP_COEFF      (0.65f)                         // default input allpass coeff
#define LOOP_ALLOP_COEFF    (0.65f)                         // default loop allpass coeff

#define HI_LOSS_FREQ        (0.3f)                          // scaled center freq for the treble loss filter 
// #define HI_LOSS_FREQ_MAX    (0.08f)
#define LO_LOSS_FREQ        (0.06f)                         // scaled center freq for the bass loss filter 

#define LFO_AMPL_BITS       (5)                             // 2^LFO_AMPL_BITS will be the LFO amplitude 
#define LFO_AMPL            ((1<<LFO_AMPL_BITS) + 1)        // lfo amplitude
#define LFO_READ_OFFSET     (LFO_AMPL>>1)                   // read offset = half the amplitude
#define LFO_FRAC_BITS       (16 - LFO_AMPL_BITS)            // fractional part used for linear interpolation
#define LFO_FRAC_MASK       ((1<<LFO_FRAC_BITS)-1)          // mask for the above

#define LFO1_FREQ_HZ        (1.37f)                          // LFO1 frequency in Hz
#define LFO2_FREQ_HZ        (1.52f)                          // LFO2 frequency in Hz

#define RV_MASTER_LOWPASS_F (0.6f)                           // master lowpass scaled frequency coeff. 

extern "C" {
extern const int16_t AudioWaveformSine[257];
}

#ifdef REVERB_USE_DMAMEM

float32_t DMAMEM input_blockL[AUDIO_BLOCK_SAMPLES];
float32_t DMAMEM input_blockR[AUDIO_BLOCK_SAMPLES];

float32_t DMAMEM in_allp1_bufL[224]; // input allpass buffers
float32_t DMAMEM in_allp2_bufL[420];
float32_t DMAMEM in_allp3_bufL[856];
float32_t DMAMEM in_allp4_bufL[1089];

float32_t DMAMEM in_allp1_bufR[156]; // input allpass buffers
float32_t DMAMEM in_allp2_bufR[520];
float32_t DMAMEM in_allp3_bufR[956];
float32_t DMAMEM in_allp4_bufR[1289];

float32_t DMAMEM lp_allp1_buf[2303]; // loop allpass buffers
float32_t DMAMEM lp_allp2_buf[2905];
float32_t DMAMEM lp_allp3_buf[3175];
float32_t DMAMEM lp_allp4_buf[2398];

float32_t DMAMEM lp_dly1_buf[3423];
float32_t DMAMEM lp_dly2_buf[4589];
float32_t DMAMEM lp_dly3_buf[4365];
float32_t DMAMEM lp_dly4_buf[3698];
#endif

AudioEffectPlateReverb::AudioEffectPlateReverb() : AudioStream(2, inputQueueArray)
{
    input_attn = 0.5f;
    in_allp_k = INP_ALLP_COEFF;

    memset(in_allp1_bufL, 0, sizeof(in_allp1_bufL));
	memset(in_allp2_bufL, 0, sizeof(in_allp2_bufL));
	memset(in_allp3_bufL, 0, sizeof(in_allp3_bufL));
	memset(in_allp4_bufL, 0, sizeof(in_allp4_bufL));
    in_allp1_idxL = 0;
    in_allp2_idxL = 0;
    in_allp3_idxL = 0;
    in_allp4_idxL = 0;

    memset(in_allp1_bufR, 0, sizeof(in_allp1_bufR));
	memset(in_allp2_bufR, 0, sizeof(in_allp2_bufR));
	memset(in_allp3_bufR, 0, sizeof(in_allp3_bufR));
	memset(in_allp4_bufR, 0, sizeof(in_allp4_bufR));
    in_allp1_idxR = 0;
    in_allp2_idxR = 0;
    in_allp3_idxR = 0;
    in_allp4_idxR = 0;

    in_allp_out_R = 0.0f;

    memset(lp_allp1_buf, 0, sizeof(lp_allp1_buf));
    memset(lp_allp2_buf, 0, sizeof(lp_allp2_buf));
    memset(lp_allp3_buf, 0, sizeof(lp_allp3_buf));
    memset(lp_allp4_buf, 0, sizeof(lp_allp4_buf));
    lp_allp1_idx = 0;
    lp_allp2_idx = 0;
    lp_allp3_idx = 0;
    lp_allp4_idx = 0;
    loop_allp_k = LOOP_ALLOP_COEFF;
    lp_allp_out = 0.0f;

    memset(lp_dly1_buf, 0, sizeof(lp_dly1_buf));
    memset(lp_dly2_buf, 0, sizeof(lp_dly2_buf));
    memset(lp_dly3_buf, 0, sizeof(lp_dly3_buf));
    memset(lp_dly4_buf, 0, sizeof(lp_dly4_buf));
    lp_dly1_idx = 0;
    lp_dly2_idx = 0;
    lp_dly3_idx = 0;
    lp_dly4_idx = 0;

    lp_hidamp_k = 1.0f;
    lp_lodamp_k = 0.0f;

    lp_lowpass_f = HI_LOSS_FREQ;
    lp_hipass_f = LO_LOSS_FREQ;

    lpf1 = 0.0f;
    lpf2 = 0.0f;
    lpf3 = 0.0f;
    lpf4 = 0.0f;

    hpf1 = 0.0f;
    hpf2 = 0.0f;
    hpf3 = 0.0f;
    hpf4 = 0.0f;

    master_lowpass_f = RV_MASTER_LOWPASS_F;
    master_lowpass_l = 0.0f;
    master_lowpass_r = 0.0f;

    lfo1_phase_acc = 0;
    lfo1_adder = (UINT32_MAX + 1)/(AUDIO_SAMPLE_RATE_EXACT * LFO1_FREQ_HZ);
    lfo2_phase_acc = 0;
    lfo2_adder = (UINT32_MAX + 1)/(AUDIO_SAMPLE_RATE_EXACT * LFO2_FREQ_HZ);  
}

// #define sat16(n, rshift) signed_saturate_rshift((n), 16, (rshift))

// TODO: move this to one of the data files, use in output_adat.cpp, output_tdm.cpp, etc
static const audio_block_t zeroblock = {
0, 0, 0, {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#if AUDIO_BLOCK_SAMPLES > 16
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 32
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 48
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 64
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 80
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 96
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 112
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
} };

void AudioEffectPlateReverb::update()
{
    const audio_block_t *blockL, *blockR;

#if defined(__ARM_ARCH_7EM__)
    audio_block_t *outblockL;
	audio_block_t *outblockR;
	int i;
	float32_t input, acc, temp1, temp2;
    uint16_t temp16;
    float32_t rv_time;

    // for LFOs:
    int16_t lfo1_out_sin, lfo1_out_cos, lfo2_out_sin, lfo2_out_cos;
    int32_t y0, y1;
    int64_t y;
    uint32_t idx;
    static bool cleanup_done = false;
    // handle bypass, 1st call will clean the buffers to avoid continuing the previous reverb tail
    if (bypass)
    {
        if (!cleanup_done)
        {
            memset(in_allp1_bufL, 0, sizeof(in_allp1_bufL));
            memset(in_allp2_bufL, 0, sizeof(in_allp2_bufL));
            memset(in_allp3_bufL, 0, sizeof(in_allp3_bufL));
            memset(in_allp4_bufL, 0, sizeof(in_allp4_bufL));
            memset(in_allp1_bufR, 0, sizeof(in_allp1_bufR));
            memset(in_allp2_bufR, 0, sizeof(in_allp2_bufR));
            memset(in_allp3_bufR, 0, sizeof(in_allp3_bufR));
            memset(in_allp4_bufR, 0, sizeof(in_allp4_bufR));
            memset(lp_allp1_buf, 0, sizeof(lp_allp1_buf));
            memset(lp_allp2_buf, 0, sizeof(lp_allp2_buf));
            memset(lp_allp3_buf, 0, sizeof(lp_allp3_buf));
            memset(lp_allp4_buf, 0, sizeof(lp_allp4_buf));
            memset(lp_dly1_buf, 0, sizeof(lp_dly1_buf));
            memset(lp_dly2_buf, 0, sizeof(lp_dly2_buf));
            memset(lp_dly3_buf, 0, sizeof(lp_dly3_buf));
            memset(lp_dly4_buf, 0, sizeof(lp_dly4_buf));

            cleanup_done = true;
        }
        blockL = receiveReadOnly(0);
        blockR = receiveReadOnly(1);
        if (!blockL) blockL = &zeroblock;
        if (!blockR) blockR = &zeroblock;
        transmit((audio_block_t *)blockL,0);
        transmit((audio_block_t *)blockR,1);
        if (blockL != &zeroblock) release((audio_block_t *)blockL);
        if (blockR != &zeroblock) release((audio_block_t *)blockR);

        return;
    }
    cleanup_done = false;


    blockL = receiveReadOnly(0);
    blockR = receiveReadOnly(1);
	outblockL = allocate();
	outblockR = allocate();
	if (!outblockL || !outblockR) {
		if (outblockL) release(outblockL);
		if (outblockR) release(outblockR);
		if (blockL) release((audio_block_t *)blockL);
        if (blockR) release((audio_block_t *)blockR);
		return;
	}

	if (!blockL) blockL = &zeroblock;
    if (!blockR) blockR = &zeroblock;

    // convert data to float32
    arm_q15_to_float((q15_t *)blockL->data, input_blockL, AUDIO_BLOCK_SAMPLES);
    arm_q15_to_float((q15_t *)blockR->data, input_blockR, AUDIO_BLOCK_SAMPLES);

    rv_time = rv_time_k;

	for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) 
    {
        // do the LFOs
        lfo1_phase_acc += lfo1_adder;
        idx = lfo1_phase_acc >> 24;     // 8bit lookup table address
        y0 =  AudioWaveformSine[idx];
        y1 = AudioWaveformSine[idx+1];
        idx = lfo1_phase_acc & 0x00FFFFFF;   // lower 24 bit = fractional part
        y = (int64_t)y0 * (0x00FFFFFF - idx);
        y += (int64_t)y1 * idx;
        lfo1_out_sin = (int32_t) (y >> (32-8)); // 16bit output
        idx = ((lfo1_phase_acc >> 24)+64) & 0xFF;
        y0 = AudioWaveformSine[idx];
        y1 = AudioWaveformSine[idx + 1];
        y = (int64_t)y0 * (0x00FFFFFF - idx);
        y += (int64_t)y1 * idx;
        lfo1_out_cos = (int32_t) (y >> (32-8)); // 16bit output        

        lfo2_phase_acc += lfo2_adder;
        idx = lfo2_phase_acc >> 24;     // 8bit lookup table address
        y0 =  AudioWaveformSine[idx];
        y1 = AudioWaveformSine[idx+1];
        idx = lfo2_phase_acc & 0x00FFFFFF;   // lower 24 bit = fractional part
        y = (int64_t)y0 * (0x00FFFFFF - idx);
        y += (int64_t)y1 * idx;
        lfo2_out_sin = (int32_t) (y >> (32-8)); //32-8->output 16bit,
        idx = ((lfo2_phase_acc >> 24)+64) & 0xFF;
        y0 = AudioWaveformSine[idx];
        y1 = AudioWaveformSine[idx + 1];
        y = (int64_t)y0 * (0x00FFFFFF - idx);
        y += (int64_t)y1 * idx;
        lfo2_out_cos = (int32_t) (y >> (32-8)); // 16bit output   

		input = input_blockL[i] * input_attn;
        // chained input allpasses, channel L
        acc = in_allp1_bufL[in_allp1_idxL]  + input * in_allp_k;  
        in_allp1_bufL[in_allp1_idxL] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp1_idxL >= sizeof(in_allp1_bufL)/sizeof(float32_t)) in_allp1_idxL = 0;

        acc = in_allp2_bufL[in_allp2_idxL]  + input * in_allp_k;  
        in_allp2_bufL[in_allp2_idxL] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp2_idxL >= sizeof(in_allp2_bufL)/sizeof(float32_t)) in_allp2_idxL = 0;

        acc = in_allp3_bufL[in_allp3_idxL]  + input * in_allp_k;  
        in_allp3_bufL[in_allp3_idxL] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp3_idxL >= sizeof(in_allp3_bufL)/sizeof(float32_t)) in_allp3_idxL = 0;

        acc = in_allp4_bufL[in_allp4_idxL]  + input * in_allp_k;  
        in_allp4_bufL[in_allp4_idxL] = input - in_allp_k * acc;
        in_allp_out_L = acc;
        if (++in_allp4_idxL >= sizeof(in_allp4_bufL)/sizeof(float32_t)) in_allp4_idxL = 0;

        input = input_blockR[i] * input_attn;

        // chained input allpasses, channel R
        acc = in_allp1_bufR[in_allp1_idxR]  + input * in_allp_k;  
        in_allp1_bufR[in_allp1_idxR] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp1_idxR >= sizeof(in_allp1_bufR)/sizeof(float32_t)) in_allp1_idxR = 0;

        acc = in_allp2_bufR[in_allp2_idxR]  + input * in_allp_k;  
        in_allp2_bufR[in_allp2_idxR] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp2_idxR >= sizeof(in_allp2_bufR)/sizeof(float32_t)) in_allp2_idxR = 0;

        acc = in_allp3_bufR[in_allp3_idxR]  + input * in_allp_k;  
        in_allp3_bufR[in_allp3_idxR] = input - in_allp_k * acc;
        input = acc;
        if (++in_allp3_idxR >= sizeof(in_allp3_bufR)/sizeof(float32_t)) in_allp3_idxR = 0;

        acc = in_allp4_bufR[in_allp4_idxR]  + input * in_allp_k;  
        in_allp4_bufR[in_allp4_idxR] = input - in_allp_k * acc;
        in_allp_out_R = acc;
        if (++in_allp4_idxR >= sizeof(in_allp4_bufR)/sizeof(float32_t)) in_allp4_idxR = 0;

        // input allpases done, start loop allpases
        input = lp_allp_out + in_allp_out_R; 
        acc = lp_allp1_buf[lp_allp1_idx] + input * loop_allp_k;                  // input is the lp allpass chain output
        lp_allp1_buf[lp_allp1_idx] = input - loop_allp_k * acc;
        input = acc;
        if (++lp_allp1_idx >= sizeof(lp_allp1_buf)/sizeof(float32_t)) lp_allp1_idx = 0;
        
        acc = lp_dly1_buf[lp_dly1_idx];                                                   // read the end of the delay
        lp_dly1_buf[lp_dly1_idx] = input;                                                 // write new sample
        input = acc;
        if (++lp_dly1_idx >= sizeof(lp_dly1_buf)/sizeof(float32_t)) lp_dly1_idx = 0;     // update index

        // hi/lo shelving filter
        temp1 = input - lpf1;
        lpf1 += temp1 * lp_lowpass_f;
        temp2 = input - lpf1;
        temp1 = lpf1 - hpf1;
        hpf1 += temp1 * lp_hipass_f;
        acc = lpf1 + temp2*lp_hidamp_k + hpf1*lp_lodamp_k;
        acc = acc * rv_time * rv_time_scaler;                                                                // scale by the reveb time
        
        input = acc + in_allp_out_L;

        acc = lp_allp2_buf[lp_allp2_idx] + input * loop_allp_k;                  
        lp_allp2_buf[lp_allp2_idx] = input - loop_allp_k * acc;
        input = acc;
        if (++lp_allp2_idx >= sizeof(lp_allp2_buf)/sizeof(float32_t)) lp_allp2_idx = 0;
        acc = lp_dly2_buf[lp_dly2_idx];                                                   // read the end of the delay
        lp_dly2_buf[lp_dly2_idx] = input;                                                 // write new sample
        input = acc;
        if (++lp_dly2_idx >= sizeof(lp_dly2_buf)/sizeof(float32_t)) lp_dly2_idx = 0;     // update index
        // hi/lo shelving filter
        temp1 = input - lpf2;
        lpf2 += temp1 * lp_lowpass_f;
        temp2 = input - lpf2;
        temp1 = lpf2 - hpf2;
        hpf2 += temp1 * lp_hipass_f;
        acc = lpf2 + temp2*lp_hidamp_k + hpf2*lp_lodamp_k;
        acc = acc * rv_time * rv_time_scaler;             

        input = acc + in_allp_out_R;

        acc = lp_allp3_buf[lp_allp3_idx] + input * loop_allp_k;                  
        lp_allp3_buf[lp_allp3_idx] = input - loop_allp_k * acc;
        input = acc;
        if (++lp_allp3_idx >= sizeof(lp_allp3_buf)/sizeof(float32_t)) lp_allp3_idx = 0;
        acc = lp_dly3_buf[lp_dly3_idx];                                                   // read the end of the delay
        lp_dly3_buf[lp_dly3_idx] = input;                                                 // write new sample
        input = acc;
        if (++lp_dly3_idx >= sizeof(lp_dly3_buf)/sizeof(float32_t)) lp_dly3_idx = 0;     // update index
        // hi/lo shelving filter
        temp1 = input - lpf3;
        lpf3 += temp1 * lp_lowpass_f;
        temp2 = input - lpf3;
        temp1 = lpf3 - hpf3;
        hpf3 += temp1 * lp_hipass_f;
        acc = lpf3 + temp2*lp_hidamp_k + hpf3*lp_lodamp_k;
        acc = acc * rv_time * rv_time_scaler;              

        input = acc + in_allp_out_L;       

        acc = lp_allp4_buf[lp_allp4_idx] + input * loop_allp_k;                  
        lp_allp4_buf[lp_allp4_idx] = input - loop_allp_k * acc;
        input = acc;
        if (++lp_allp4_idx >= sizeof(lp_allp4_buf)/sizeof(float32_t)) lp_allp4_idx = 0;
        acc = lp_dly4_buf[lp_dly4_idx];                                                   // read the end of the delay
        lp_dly4_buf[lp_dly4_idx] = input;                                                 // write new sample
        input = acc;
        if (++lp_dly4_idx >= sizeof(lp_dly4_buf)/sizeof(float32_t)) lp_dly4_idx= 0;     // update index
        // hi/lo shelving filter
        temp1 = input - lpf4;
        lpf4 += temp1 * lp_lowpass_f;
        temp2 = input - lpf4;
        temp1 = lpf4 - hpf4;
        hpf4 += temp1 * lp_hipass_f;
        acc = lpf4 + temp2*lp_hidamp_k + hpf4*lp_lodamp_k;
        acc = acc * rv_time * rv_time_scaler;              

        lp_allp_out = acc;

        // channel L:
#ifdef TAP1_MODULATED
        temp16 = (lp_dly1_idx + lp_dly1_offset_L + (lfo1_out_cos>>LFO_FRAC_BITS)) %  (sizeof(lp_dly1_buf)/sizeof(float32_t));
        temp1 = lp_dly1_buf[temp16++];    // sample now
        if (temp16  >= sizeof(lp_dly1_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly1_buf[temp16];    // sample next
        input = (float32_t)(lfo1_out_cos & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc = (temp1*(1.0f-input) + temp2*input)* 0.8f;
#else
        temp16 = (lp_dly1_idx + lp_dly1_offset_L) %  (sizeof(lp_dly1_buf)/sizeof(float32_t));
        acc = lp_dly1_buf[temp16]* 0.8f;
#endif


#ifdef TAP2_MODULATED
        temp16 = (lp_dly2_idx + lp_dly2_offset_L + (lfo1_out_sin>>LFO_FRAC_BITS)) % (sizeof(lp_dly2_buf)/sizeof(float32_t));
        temp1 = lp_dly2_buf[temp16++];
        if (temp16  >= sizeof(lp_dly2_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly2_buf[temp16]; 
        input = (float32_t)(lfo1_out_sin & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.7f;
#else
        temp16 = (lp_dly2_idx + lp_dly2_offset_L) % (sizeof(lp_dly2_buf)/sizeof(float32_t));
        acc += (temp1*(1.0f-input) + temp2*input)* 0.6f;
#endif

        temp16 = (lp_dly3_idx + lp_dly3_offset_L + (lfo2_out_cos>>LFO_FRAC_BITS)) % (sizeof(lp_dly3_buf)/sizeof(float32_t));
        temp1 = lp_dly3_buf[temp16++];
        if (temp16  >= sizeof(lp_dly3_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly3_buf[temp16]; 
        input = (float32_t)(lfo2_out_cos & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.6f;

        temp16 = (lp_dly4_idx + lp_dly4_offset_L + (lfo2_out_sin>>LFO_FRAC_BITS)) % (sizeof(lp_dly4_buf)/sizeof(float32_t));
        temp1 = lp_dly4_buf[temp16++];
        if (temp16  >= sizeof(lp_dly4_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly4_buf[temp16]; 
        input = (float32_t)(lfo2_out_sin & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.5f;

        // Master lowpass filter
        temp1 = acc - master_lowpass_l;
        master_lowpass_l += temp1 * master_lowpass_f;

        outblockL->data[i] =(int16_t)(master_lowpass_l * 32767.0f); //sat16(output * 30, 0);

        // Channel R
#ifdef TAP1_MODULATED
        temp16 = (lp_dly1_idx + lp_dly1_offset_R + (lfo1_out_sin>>LFO_FRAC_BITS)) %  (sizeof(lp_dly1_buf)/sizeof(float32_t));
        temp1 = lp_dly1_buf[temp16++];    // sample now
        if (temp16  >= sizeof(lp_dly1_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly1_buf[temp16];    // sample next
        input = (float32_t)(lfo1_out_sin & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k

        acc = (temp1*(1.0f-input) + temp2*input)* 0.8f;
#else
        temp16 = (lp_dly1_idx + lp_dly1_offset_R) %  (sizeof(lp_dly1_buf)/sizeof(float32_t));
        acc = lp_dly1_buf[temp16] * 0.8f;
#endif
#ifdef TAP2_MODULATED
        temp16 = (lp_dly2_idx + lp_dly2_offset_R + (lfo1_out_cos>>LFO_FRAC_BITS)) % (sizeof(lp_dly2_buf)/sizeof(float32_t));
        temp1 = lp_dly2_buf[temp16++];
        if (temp16  >= sizeof(lp_dly2_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly2_buf[temp16]; 
        input = (float32_t)(lfo1_out_cos & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.7f;
#else
        temp16 = (lp_dly2_idx + lp_dly2_offset_R) % (sizeof(lp_dly2_buf)/sizeof(float32_t));
        acc += (temp1*(1.0f-input) + temp2*input)* 0.7f;
#endif
        temp16 = (lp_dly3_idx + lp_dly3_offset_R + (lfo2_out_sin>>LFO_FRAC_BITS)) % (sizeof(lp_dly3_buf)/sizeof(float32_t));
        temp1 = lp_dly3_buf[temp16++];
        if (temp16  >= sizeof(lp_dly3_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly3_buf[temp16]; 
        input = (float32_t)(lfo2_out_sin & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.6f;

        temp16 = (lp_dly4_idx + lp_dly4_offset_R + (lfo2_out_cos>>LFO_FRAC_BITS)) % (sizeof(lp_dly4_buf)/sizeof(float32_t));
        temp1 = lp_dly4_buf[temp16++];
        if (temp16  >= sizeof(lp_dly4_buf)/sizeof(float32_t)) temp16 = 0;
        temp2 = lp_dly4_buf[temp16]; 
        input = (float32_t)(lfo2_out_cos & LFO_FRAC_MASK) / ((float32_t)LFO_FRAC_MASK); // interp. k
        acc += (temp1*(1.0f-input) + temp2*input)* 0.5f;

        // Master lowpass filter
        temp1 = acc - master_lowpass_r;
        master_lowpass_r += temp1 * master_lowpass_f;
        outblockR->data[i] =(int16_t)(master_lowpass_r * 32767.0f);
		
	}
    transmit(outblockL, 0);
	transmit(outblockR, 1);
	release(outblockL);
	release(outblockR);
	if (blockL != &zeroblock) release((audio_block_t *)blockL);
    if (blockR != &zeroblock) release((audio_block_t *)blockR);

#elif defined(KINETISL)
	blockL = receiveReadOnly(0);
	if (blockL) release(blockL);
    blockR = receiveReadOnly(1);
    if (blockR) release(blockR);
#endif
}
