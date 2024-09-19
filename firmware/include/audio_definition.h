#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       string_vibrato_1; //xy=840,1119
AudioEffectEnvelope      envelope_string_vibrato_1; //xy=1059,1120
AudioSynthWaveform       chords_vibrato_lfo; //xy=1191,1621
AudioSynthWaveformModulated waveform_string_4; //xy=1324,1224
AudioSynthWaveformModulated waveform_string_2; //xy=1325,1156
AudioSynthWaveformModulated waveform_string_5; //xy=1325,1261
AudioSynthWaveformModulated waveform_string_7; //xy=1325,1332
AudioSynthWaveformModulated waveform_string_10; //xy=1325,1439
AudioSynthWaveformModulated waveform_string_3; //xy=1326,1190
AudioSynthWaveformModulated waveform_string_12; //xy=1325,1506
AudioSynthWaveformModulated waveform_string_6; //xy=1326,1298
AudioSynthWaveformModulated waveform_string_8; //xy=1326,1368
AudioSynthWaveformModulated waveform_string_1; //xy=1327,1121
AudioSynthWaveformModulated waveform_string_9; //xy=1327,1404
AudioSynthWaveformModulated waveform_string_11; //xy=1327,1474
AudioSynthWaveformDc     filter_dc;      //xy=1350,690
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=1410,2455
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=1417,2190
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=1420,1903
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=1434,1621
AudioEffectEnvelope      envelope_filter_2; //xy=1542,721
AudioEffectEnvelope      envelope_filter_3; //xy=1542,751
AudioEffectEnvelope      envelope_filter_5; //xy=1542,813
AudioEffectEnvelope      envelope_filter_6; //xy=1542,845
AudioEffectEnvelope      envelope_filter_7; //xy=1542,876
AudioEffectEnvelope      envelope_filter_1; //xy=1543,690
AudioEffectEnvelope      envelope_filter_4; //xy=1543,781
AudioEffectEnvelope      envelope_filter_12; //xy=1542,1034
AudioEffectEnvelope      envelope_filter_9; //xy=1543,939
AudioEffectEnvelope      envelope_filter_11; //xy=1543,1003
AudioEffectEnvelope      envelope_filter_10; //xy=1546,972
AudioEffectEnvelope      envelope_filter_8; //xy=1547,906
AudioEffectEnvelope      envelope_string_6; //xy=1547,1296
AudioEffectEnvelope      envelope_string_8; //xy=1547,1367
AudioEffectEnvelope      envelope_string_10; //xy=1547,1435
AudioEffectEnvelope      envelope_string_12; //xy=1547,1504
AudioEffectEnvelope      envelope_string_5; //xy=1548,1261
AudioEffectEnvelope      envelope_string_7; //xy=1548,1331
AudioEffectEnvelope      envelope_string_9; //xy=1548,1401
AudioEffectEnvelope      envelope_string_3; //xy=1549,1190
AudioEffectEnvelope      envelope_string_4; //xy=1550,1224
AudioEffectEnvelope      envelope_string_11; //xy=1550,1470
AudioEffectEnvelope      envelope_string_1; //xy=1552,1123
AudioEffectEnvelope      envelope_string_2; //xy=1552,1156
AudioSynthNoiseWhite     voice3_noise;   //xy=1646,2290
AudioSynthWaveformModulated voice3_osc3;    //xy=1649,2256
AudioSynthNoiseWhite     voice4_noise;   //xy=1649,2567
AudioSynthWaveformModulated voice3_osc2;    //xy=1651,2220
AudioSynthWaveformModulated voice4_osc3;    //xy=1651,2530
AudioSynthWaveformModulated voice3_osc1;    //xy=1655,2182
AudioSynthWaveformModulated voice4_osc2;    //xy=1654,2493
AudioSynthWaveformModulated voice4_osc1;    //xy=1656,2458
AudioSynthWaveform       chords_filter_LFO; //xy=1669,1813
AudioSynthNoiseWhite     voice2_noise;   //xy=1672,2013
AudioSynthWaveformModulated voice2_osc3;    //xy=1674,1976
AudioSynthWaveformModulated voice2_osc2;    //xy=1676,1940
AudioSynthWaveformModulated voice2_osc1;    //xy=1677,1903
AudioSynthNoiseWhite     voice1_noise;   //xy=1685,1731
AudioSynthWaveformModulated voice1_osc3;    //xy=1690,1695
AudioSynthWaveformModulated voice1_osc1;    //xy=1691,1625
AudioSynthWaveformModulated voice1_osc2;    //xy=1691,1660
AudioFilterStateVariable filter_string_4; //xy=1784,1191
AudioFilterStateVariable filter_string_5; //xy=1784,1237
AudioFilterStateVariable filter_string_1; //xy=1785,1056
AudioFilterStateVariable filter_string_7; //xy=1784,1329
AudioFilterStateVariable filter_string_2; //xy=1785,1101
AudioFilterStateVariable filter_string_8; //xy=1785,1376
AudioFilterStateVariable filter_string_3; //xy=1786,1146
AudioFilterStateVariable filter_string_6; //xy=1787,1286
AudioFilterStateVariable filter_string_9; //xy=1787,1421
AudioFilterStateVariable filter_string_11; //xy=1788,1513
AudioFilterStateVariable filter_string_12; //xy=1788,1559
AudioFilterStateVariable filter_string_10; //xy=1789,1466
AudioEffectEnvelope      voice4_envelope_filter; //xy=1848,2633
AudioMixer4              voice3_mixer;   //xy=1852,2214
AudioMixer4              voice4_mixer;   //xy=1853,2477
AudioMixer4              voice2_mixer;   //xy=1857,1929
AudioEffectEnvelope      voice3_envelope_filter; //xy=1856,2320
AudioEffectEnvelope      voice2_envelope_filter; //xy=1872,2047
AudioEffectEnvelope      voice1_envelope_filter; //xy=1874,1816
AudioMixer4              voice1_mixer;   //xy=1876,1659
AudioMixer4              string_mix_3;   //xy=2042,1449
AudioMixer4              string_mix_1;   //xy=2045,1137
AudioMixer4              string_mix_2;   //xy=2059,1266
AudioSynthWaveform       chords_tremolo_lfo; //xy=2124,1817
AudioFilterStateVariable voice4_filter;  //xy=2157,2481
AudioFilterStateVariable voice2_filter;  //xy=2165,1934
AudioFilterStateVariable voice3_filter;  //xy=2164,2210
AudioFilterStateVariable voice1_filter;  //xy=2186,1666
AudioMixer4              all_string_mix; //xy=2259,1266
AudioEffectEnvelope      voice1_envelope; //xy=2342,1817
AudioEffectEnvelope      voice3_envelope; //xy=2344,2348
AudioEffectEnvelope      voice2_envelope; //xy=2347,2068
AudioEffectEnvelope      voice4_envelope; //xy=2351,2667
AudioEffectMultiply      voice2_tremolo_mult; //xy=2390,1930
AudioEffectMultiply      voice3_tremolo_mult; //xy=2392,2210
AudioEffectMultiply      voice4_tremolo_mult; //xy=2395,2485
AudioEffectMultiply      voice1_tremolo_mult; //xy=2400,1660
AudioEffectWaveshaper    string_waveshape;     //xy=2492.200012207031,1084.200012207031
AudioMixer4              string_waveshaper_mix;         //xy=2502.2003173828125,1264.199951171875
AudioMixer4              chord_voice_mixer; //xy=2701,1681
AudioMixer4              string_delay_mix; //xy=2798,1160
AudioFilterStateVariable filter_delay_strings; //xy=2800,1234
AudioSynthWaveform       string_tremolo_lfo; //xy=2823,1435
AudioEffectWaveshaper    chord_waveshape; //xy=2879,1577
AudioMixer4              chord_waveshaper_mix; //xy=2964,1680
AudioEffectDelay         delay_strings;  //xy=3014,1197
AudioMixer4              strings_effect_mix; //xy=3034,1317
AudioEffectMultiply      string_multiply; //xy=3149,1420
AudioSynthWaveform       string_filter_lfo; //xy=3186,1500
AudioMixer4              chord_delay_mix; //xy=3238,1647
AudioFilterStateVariable filter_delay_chords; //xy=3244,1729
AudioMixer4              chords_effect_mix; //xy=3280,1811
AudioFilterStateVariable string_filter;  //xy=3357,1422
AudioEffectDelay         delay_chords;   //xy=3450,1655
AudioMixer4              string_filter_mixer; //xy=3542,1425
AudioFilterStateVariable chords_main_filter; //xy=3644,1726
AudioInputI2S            i2s1;           //xy=3727,1185
AudioMixer4              chords_main_filter_mixer; //xy=3872,1733
AudioConnection          patchCord1(string_vibrato_1, envelope_string_vibrato_1);
AudioConnection          patchCord2(envelope_string_vibrato_1, 0, waveform_string_1, 0);
AudioConnection          patchCord3(envelope_string_vibrato_1, 0, waveform_string_3, 0);
AudioConnection          patchCord4(envelope_string_vibrato_1, 0, waveform_string_5, 0);
AudioConnection          patchCord5(envelope_string_vibrato_1, 0, waveform_string_7, 0);
AudioConnection          patchCord6(envelope_string_vibrato_1, 0, waveform_string_9, 0);
AudioConnection          patchCord7(envelope_string_vibrato_1, 0, waveform_string_11, 0);
AudioConnection          patchCord8(envelope_string_vibrato_1, 0, waveform_string_2, 0);
AudioConnection          patchCord9(envelope_string_vibrato_1, 0, waveform_string_4, 0);
AudioConnection          patchCord10(envelope_string_vibrato_1, 0, waveform_string_6, 0);
AudioConnection          patchCord11(envelope_string_vibrato_1, 0, waveform_string_8, 0);
AudioConnection          patchCord12(envelope_string_vibrato_1, 0, waveform_string_10, 0);
AudioConnection          patchCord13(envelope_string_vibrato_1, 0, waveform_string_12, 0);
AudioConnection          patchCord14(chords_vibrato_lfo, voice1_vibrato_envelope);
AudioConnection          patchCord15(chords_vibrato_lfo, voice2_vibrato_envelope);
AudioConnection          patchCord16(chords_vibrato_lfo, voice3_vibrato_envelope);
AudioConnection          patchCord17(chords_vibrato_lfo, voice4_vibrato_envelope);
AudioConnection          patchCord18(waveform_string_4, envelope_string_4);
AudioConnection          patchCord19(waveform_string_2, envelope_string_2);
AudioConnection          patchCord20(waveform_string_5, envelope_string_5);
AudioConnection          patchCord21(waveform_string_7, envelope_string_7);
AudioConnection          patchCord22(waveform_string_10, envelope_string_10);
AudioConnection          patchCord23(waveform_string_3, envelope_string_3);
AudioConnection          patchCord24(waveform_string_12, envelope_string_12);
AudioConnection          patchCord25(waveform_string_6, envelope_string_6);
AudioConnection          patchCord26(waveform_string_8, envelope_string_8);
AudioConnection          patchCord27(waveform_string_1, envelope_string_1);
AudioConnection          patchCord28(waveform_string_9, envelope_string_9);
AudioConnection          patchCord29(waveform_string_11, envelope_string_11);
AudioConnection          patchCord30(filter_dc, envelope_filter_1);
AudioConnection          patchCord31(filter_dc, envelope_filter_2);
AudioConnection          patchCord32(filter_dc, envelope_filter_3);
AudioConnection          patchCord33(filter_dc, envelope_filter_4);
AudioConnection          patchCord34(filter_dc, envelope_filter_5);
AudioConnection          patchCord35(filter_dc, envelope_filter_6);
AudioConnection          patchCord36(filter_dc, envelope_filter_7);
AudioConnection          patchCord37(filter_dc, envelope_filter_8);
AudioConnection          patchCord38(filter_dc, envelope_filter_9);
AudioConnection          patchCord39(filter_dc, envelope_filter_10);
AudioConnection          patchCord40(filter_dc, envelope_filter_11);
AudioConnection          patchCord41(filter_dc, envelope_filter_12);
AudioConnection          patchCord42(voice4_vibrato_envelope, 0, voice4_osc1, 0);
AudioConnection          patchCord43(voice4_vibrato_envelope, 0, voice4_osc2, 0);
AudioConnection          patchCord44(voice4_vibrato_envelope, 0, voice4_osc3, 0);
AudioConnection          patchCord45(voice3_vibrato_envelope, 0, voice3_osc1, 0);
AudioConnection          patchCord46(voice3_vibrato_envelope, 0, voice3_osc2, 0);
AudioConnection          patchCord47(voice3_vibrato_envelope, 0, voice3_osc3, 0);
AudioConnection          patchCord48(voice2_vibrato_envelope, 0, voice2_osc1, 0);
AudioConnection          patchCord49(voice2_vibrato_envelope, 0, voice2_osc2, 0);
AudioConnection          patchCord50(voice2_vibrato_envelope, 0, voice2_osc3, 0);
AudioConnection          patchCord51(voice1_vibrato_envelope, 0, voice1_osc1, 0);
AudioConnection          patchCord52(voice1_vibrato_envelope, 0, voice1_osc2, 0);
AudioConnection          patchCord53(voice1_vibrato_envelope, 0, voice1_osc3, 0);
AudioConnection          patchCord54(envelope_filter_2, 0, filter_string_2, 1);
AudioConnection          patchCord55(envelope_filter_3, 0, filter_string_3, 1);
AudioConnection          patchCord56(envelope_filter_5, 0, filter_string_5, 1);
AudioConnection          patchCord57(envelope_filter_6, 0, filter_string_6, 1);
AudioConnection          patchCord58(envelope_filter_7, 0, filter_string_7, 1);
AudioConnection          patchCord59(envelope_filter_1, 0, filter_string_1, 1);
AudioConnection          patchCord60(envelope_filter_4, 0, filter_string_4, 1);
AudioConnection          patchCord61(envelope_filter_12, 0, filter_string_12, 1);
AudioConnection          patchCord62(envelope_filter_9, 0, filter_string_9, 1);
AudioConnection          patchCord63(envelope_filter_11, 0, filter_string_11, 1);
AudioConnection          patchCord64(envelope_filter_10, 0, filter_string_10, 1);
AudioConnection          patchCord65(envelope_filter_8, 0, filter_string_8, 1);
AudioConnection          patchCord66(envelope_string_6, 0, filter_string_6, 0);
AudioConnection          patchCord67(envelope_string_8, 0, filter_string_8, 0);
AudioConnection          patchCord68(envelope_string_10, 0, filter_string_10, 0);
AudioConnection          patchCord69(envelope_string_12, 0, filter_string_12, 0);
AudioConnection          patchCord70(envelope_string_5, 0, filter_string_5, 0);
AudioConnection          patchCord71(envelope_string_7, 0, filter_string_7, 0);
AudioConnection          patchCord72(envelope_string_9, 0, filter_string_9, 0);
AudioConnection          patchCord73(envelope_string_3, 0, filter_string_3, 0);
AudioConnection          patchCord74(envelope_string_4, 0, filter_string_4, 0);
AudioConnection          patchCord75(envelope_string_11, 0, filter_string_11, 0);
AudioConnection          patchCord76(envelope_string_1, 0, filter_string_1, 0);
AudioConnection          patchCord77(envelope_string_2, 0, filter_string_2, 0);
AudioConnection          patchCord78(voice3_noise, 0, voice3_mixer, 3);
AudioConnection          patchCord79(voice3_osc3, 0, voice3_mixer, 2);
AudioConnection          patchCord80(voice4_noise, 0, voice4_mixer, 3);
AudioConnection          patchCord81(voice3_osc2, 0, voice3_mixer, 1);
AudioConnection          patchCord82(voice4_osc3, 0, voice4_mixer, 2);
AudioConnection          patchCord83(voice3_osc1, 0, voice3_mixer, 0);
AudioConnection          patchCord84(voice4_osc2, 0, voice4_mixer, 1);
AudioConnection          patchCord85(voice4_osc1, 0, voice4_mixer, 0);
AudioConnection          patchCord86(chords_filter_LFO, voice1_envelope_filter);
AudioConnection          patchCord87(chords_filter_LFO, voice2_envelope_filter);
AudioConnection          patchCord88(chords_filter_LFO, voice3_envelope_filter);
AudioConnection          patchCord89(chords_filter_LFO, voice4_envelope_filter);
AudioConnection          patchCord90(voice2_noise, 0, voice2_mixer, 3);
AudioConnection          patchCord91(voice2_osc3, 0, voice2_mixer, 2);
AudioConnection          patchCord92(voice2_osc2, 0, voice2_mixer, 1);
AudioConnection          patchCord93(voice2_osc1, 0, voice2_mixer, 0);
AudioConnection          patchCord94(voice1_noise, 0, voice1_mixer, 3);
AudioConnection          patchCord95(voice1_osc3, 0, voice1_mixer, 2);
AudioConnection          patchCord96(voice1_osc1, 0, voice1_mixer, 0);
AudioConnection          patchCord97(voice1_osc2, 0, voice1_mixer, 1);
AudioConnection          patchCord98(filter_string_4, 0, string_mix_1, 3);
AudioConnection          patchCord99(filter_string_5, 0, string_mix_2, 0);
AudioConnection          patchCord100(filter_string_1, 0, string_mix_1, 0);
AudioConnection          patchCord101(filter_string_7, 0, string_mix_2, 2);
AudioConnection          patchCord102(filter_string_2, 0, string_mix_1, 1);
AudioConnection          patchCord103(filter_string_8, 0, string_mix_2, 3);
AudioConnection          patchCord104(filter_string_3, 0, string_mix_1, 2);
AudioConnection          patchCord105(filter_string_6, 0, string_mix_2, 1);
AudioConnection          patchCord106(filter_string_9, 0, string_mix_3, 0);
AudioConnection          patchCord107(filter_string_11, 0, string_mix_3, 2);
AudioConnection          patchCord108(filter_string_12, 0, string_mix_3, 3);
AudioConnection          patchCord109(filter_string_10, 0, string_mix_3, 1);
AudioConnection          patchCord110(voice4_envelope_filter, 0, voice4_filter, 1);
AudioConnection          patchCord111(voice3_mixer, 0, voice3_filter, 0);
AudioConnection          patchCord112(voice4_mixer, 0, voice4_filter, 0);
AudioConnection          patchCord113(voice2_mixer, 0, voice2_filter, 0);
AudioConnection          patchCord114(voice3_envelope_filter, 0, voice3_filter, 1);
AudioConnection          patchCord115(voice2_envelope_filter, 0, voice2_filter, 1);
AudioConnection          patchCord116(voice1_envelope_filter, 0, voice1_filter, 1);
AudioConnection          patchCord117(voice1_mixer, 0, voice1_filter, 0);
AudioConnection          patchCord118(string_mix_3, 0, all_string_mix, 2);
AudioConnection          patchCord119(string_mix_1, 0, all_string_mix, 0);
AudioConnection          patchCord120(string_mix_2, 0, all_string_mix, 1);
AudioConnection          patchCord121(chords_tremolo_lfo, voice1_envelope);
AudioConnection          patchCord122(chords_tremolo_lfo, voice2_envelope);
AudioConnection          patchCord123(chords_tremolo_lfo, voice3_envelope);
AudioConnection          patchCord124(chords_tremolo_lfo, voice4_envelope);
AudioConnection          patchCord125(voice4_filter, 0, voice4_tremolo_mult, 0);
AudioConnection          patchCord126(voice2_filter, 0, voice2_tremolo_mult, 0);
AudioConnection          patchCord127(voice3_filter, 0, voice3_tremolo_mult, 0);
AudioConnection          patchCord128(voice1_filter, 0, voice1_tremolo_mult, 0);
AudioConnection          patchCord129(all_string_mix, string_waveshape);
AudioConnection          patchCord130(all_string_mix, 0, string_waveshaper_mix, 0);
AudioConnection          patchCord131(voice1_envelope, 0, voice1_tremolo_mult, 1);
AudioConnection          patchCord132(voice3_envelope, 0, voice3_tremolo_mult, 1);
AudioConnection          patchCord133(voice2_envelope, 0, voice2_tremolo_mult, 1);
AudioConnection          patchCord134(voice4_envelope, 0, voice4_tremolo_mult, 1);
AudioConnection          patchCord135(voice2_tremolo_mult, 0, chord_voice_mixer, 1);
AudioConnection          patchCord136(voice3_tremolo_mult, 0, chord_voice_mixer, 2);
AudioConnection          patchCord137(voice4_tremolo_mult, 0, chord_voice_mixer, 3);
AudioConnection          patchCord138(voice1_tremolo_mult, 0, chord_voice_mixer, 0);
AudioConnection          patchCord139(string_waveshape, 0, string_waveshaper_mix, 1);
AudioConnection          patchCord140(string_waveshaper_mix, 0, strings_effect_mix, 0);
AudioConnection          patchCord141(string_waveshaper_mix, 0, string_delay_mix, 0);
AudioConnection          patchCord142(chord_voice_mixer, chord_waveshape);
AudioConnection          patchCord143(chord_voice_mixer, 0, chord_waveshaper_mix, 0);
AudioConnection          patchCord144(string_delay_mix, delay_strings);
AudioConnection          patchCord145(string_delay_mix, 0, strings_effect_mix, 1);
AudioConnection          patchCord146(filter_delay_strings, 0, string_delay_mix, 1);
AudioConnection          patchCord147(filter_delay_strings, 1, string_delay_mix, 2);
AudioConnection          patchCord148(filter_delay_strings, 2, string_delay_mix, 3);
AudioConnection          patchCord149(string_tremolo_lfo, 0, string_multiply, 1);
AudioConnection          patchCord150(chord_waveshape, 0, chord_waveshaper_mix, 1);
AudioConnection          patchCord151(chord_waveshaper_mix, 0, chord_delay_mix, 0);
AudioConnection          patchCord152(chord_waveshaper_mix, 0, chords_effect_mix, 0);
AudioConnection          patchCord153(delay_strings, 0, filter_delay_strings, 0);
AudioConnection          patchCord154(strings_effect_mix, 0, string_multiply, 0);
AudioConnection          patchCord155(string_multiply, 0, string_filter, 0);
AudioConnection          patchCord156(string_filter_lfo, 0, string_filter, 1);
AudioConnection          patchCord157(chord_delay_mix, delay_chords);
AudioConnection          patchCord158(chord_delay_mix, 0, chords_effect_mix, 1);
AudioConnection          patchCord159(filter_delay_chords, 0, chord_delay_mix, 1);
AudioConnection          patchCord160(filter_delay_chords, 1, chord_delay_mix, 2);
AudioConnection          patchCord161(filter_delay_chords, 2, chord_delay_mix, 3);
AudioConnection          patchCord162(chords_effect_mix, 0, chords_main_filter, 0);
AudioConnection          patchCord163(string_filter, 0, string_filter_mixer, 0);
AudioConnection          patchCord164(string_filter, 1, string_filter_mixer, 1);
AudioConnection          patchCord165(string_filter, 2, string_filter_mixer, 2);
AudioConnection          patchCord166(delay_chords, 0, filter_delay_chords, 0);
AudioConnection          patchCord167(chords_main_filter, 0, chords_main_filter_mixer, 0);
AudioConnection          patchCord168(chords_main_filter, 1, chords_main_filter_mixer, 1);
AudioConnection          patchCord169(chords_main_filter, 2, chords_main_filter_mixer, 2);
// GUItool: end automatically generated code

//MANUAL OUTPUT SECTION
#include "effect_platervbstereo.h"
AudioMixer4              stereo_l_mixer;        
AudioMixer4              stereo_r_mixer;   
AudioMixer4              reverb_mixer;   
AudioAmplifier           string_gain;          
AudioAmplifier           chords_gain;          

AudioOutputI2S           DAC_out;    
AudioOutputUSB           USB_out;      
AudioEffectPlateReverb   main_reverb;        

AudioConnection          patchCord2000(string_filter_mixer, 0, string_gain, 0);
AudioConnection          patchCord2001(string_gain, 0, stereo_r_mixer, 0);
AudioConnection          patchCord2002(string_gain, 0, stereo_l_mixer, 0);
AudioConnection          patchCord2003(string_gain, 0, reverb_mixer, 0);

AudioConnection          patchCord2004(chords_main_filter_mixer, 0, chords_gain, 0);
AudioConnection          patchCord2005(chords_gain, 0, stereo_r_mixer, 1);
AudioConnection          patchCord2006(chords_gain, 0, stereo_l_mixer, 1);
AudioConnection          patchCord2007(chords_gain, 0, reverb_mixer, 1);

AudioConnection          patchCord2008(reverb_mixer, 0, main_reverb, 0);
AudioConnection          patchCord2009(main_reverb, 0, stereo_r_mixer, 2);
AudioConnection          patchCord20010(main_reverb, 1, stereo_l_mixer, 2);

AudioConnection          patchCord2011(stereo_l_mixer, 0, DAC_out, 1);
AudioConnection          patchCord2012(stereo_r_mixer, 0, DAC_out, 0);

AudioConnection          patchCord2013(stereo_l_mixer, 0, USB_out, 1);
AudioConnection          patchCord2014(stereo_r_mixer, 0, USB_out, 0);

