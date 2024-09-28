#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       string_vibrato_1; //xy=1420.5,1194.4000244140625
AudioEffectEnvelope      envelope_string_vibrato_1; //xy=1639.5,1195.4000244140625
AudioSynthWaveform       chords_vibrato_lfo; //xy=1771.5,1696.4000244140625
AudioSynthWaveformModulated waveform_string_4; //xy=1904.5,1299.4000244140625
AudioSynthWaveformModulated waveform_string_2; //xy=1905.5,1231.4000244140625
AudioSynthWaveformModulated waveform_string_5; //xy=1905.5,1336.4000244140625
AudioSynthWaveformModulated waveform_string_7; //xy=1905.5,1407.4000244140625
AudioSynthWaveformModulated waveform_string_10; //xy=1905.5,1514.4000244140625
AudioSynthWaveformModulated waveform_string_3; //xy=1906.5,1265.4000244140625
AudioSynthWaveformModulated waveform_string_12; //xy=1905.5,1581.4000244140625
AudioSynthWaveformModulated waveform_string_6; //xy=1906.5,1373.4000244140625
AudioSynthWaveformModulated waveform_string_8; //xy=1906.5,1443.4000244140625
AudioSynthWaveformModulated waveform_string_1; //xy=1907.5,1196.4000244140625
AudioSynthWaveformModulated waveform_string_9; //xy=1907.5,1479.4000244140625
AudioSynthWaveformModulated waveform_string_11; //xy=1907.5,1549.4000244140625
AudioSynthWaveformDc     filter_dc;      //xy=1930.5,765.4000244140625
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=1990.5,2530.4000244140625
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=1997.5,2265.4000244140625
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=2000.5,1978.4000244140625
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=2014.5,1696.4000244140625
AudioEffectEnvelope      envelope_filter_2; //xy=2122.5,796.4000244140625
AudioEffectEnvelope      envelope_filter_3; //xy=2122.5,826.4000244140625
AudioEffectEnvelope      envelope_filter_5; //xy=2122.5,888.4000244140625
AudioEffectEnvelope      envelope_filter_6; //xy=2122.5,920.4000244140625
AudioEffectEnvelope      envelope_filter_7; //xy=2122.5,951.4000244140625
AudioEffectEnvelope      envelope_filter_1; //xy=2123.5,765.4000244140625
AudioEffectEnvelope      envelope_filter_4; //xy=2123.5,856.4000244140625
AudioEffectEnvelope      envelope_filter_12; //xy=2122.5,1109.4000244140625
AudioEffectEnvelope      envelope_filter_9; //xy=2123.5,1014.4000244140625
AudioEffectEnvelope      envelope_filter_11; //xy=2123.5,1078.4000244140625
AudioEffectEnvelope      envelope_filter_10; //xy=2126.5,1047.4000244140625
AudioEffectEnvelope      envelope_filter_8; //xy=2127.5,981.4000244140625
AudioEffectEnvelope      envelope_string_6; //xy=2127.5,1371.4000244140625
AudioEffectEnvelope      envelope_string_8; //xy=2127.5,1442.4000244140625
AudioEffectEnvelope      envelope_string_10; //xy=2127.5,1510.4000244140625
AudioEffectEnvelope      envelope_string_12; //xy=2127.5,1579.4000244140625
AudioEffectEnvelope      envelope_string_5; //xy=2128.5,1336.4000244140625
AudioEffectEnvelope      envelope_string_7; //xy=2128.5,1406.4000244140625
AudioEffectEnvelope      envelope_string_9; //xy=2128.5,1476.4000244140625
AudioEffectEnvelope      envelope_string_3; //xy=2129.5,1265.4000244140625
AudioEffectEnvelope      envelope_string_4; //xy=2130.5,1299.4000244140625
AudioEffectEnvelope      envelope_string_11; //xy=2130.5,1545.4000244140625
AudioEffectEnvelope      envelope_string_1; //xy=2132.5,1198.4000244140625
AudioEffectEnvelope      envelope_string_2; //xy=2132.5,1231.4000244140625
AudioSynthNoiseWhite     voice3_noise;   //xy=2226.5,2365.4000244140625
AudioSynthWaveformModulated voice3_osc3;    //xy=2229.5,2331.4000244140625
AudioSynthNoiseWhite     voice4_noise;   //xy=2229.5,2642.4000244140625
AudioSynthWaveformModulated voice3_osc2;    //xy=2231.5,2295.4000244140625
AudioSynthWaveformModulated voice4_osc3;    //xy=2231.5,2605.4000244140625
AudioSynthWaveformModulated voice3_osc1;    //xy=2235.5,2257.4000244140625
AudioSynthWaveformModulated voice4_osc2;    //xy=2234.5,2568.4000244140625
AudioSynthWaveformModulated voice4_osc1;    //xy=2236.5,2533.4000244140625
AudioSynthWaveform       chords_filter_LFO; //xy=2249.5,1888.4000244140625
AudioSynthNoiseWhite     voice2_noise;   //xy=2252.5,2088.4000244140625
AudioSynthWaveformModulated voice2_osc3;    //xy=2254.5,2051.4000244140625
AudioSynthWaveformModulated voice2_osc2;    //xy=2256.5,2015.4000244140625
AudioSynthWaveformModulated voice2_osc1;    //xy=2257.5,1978.4000244140625
AudioSynthNoiseWhite     voice1_noise;   //xy=2265.5,1806.4000244140625
AudioSynthWaveformModulated voice1_osc3;    //xy=2270.5,1770.4000244140625
AudioSynthWaveformModulated voice1_osc1;    //xy=2271.5,1700.4000244140625
AudioSynthWaveformModulated voice1_osc2;    //xy=2271.5,1735.4000244140625
AudioFilterStateVariable filter_string_4; //xy=2364.5,1266.4000244140625
AudioFilterStateVariable filter_string_5; //xy=2364.5,1312.4000244140625
AudioFilterStateVariable filter_string_1; //xy=2365.5,1131.4000244140625
AudioFilterStateVariable filter_string_7; //xy=2364.5,1404.4000244140625
AudioFilterStateVariable filter_string_2; //xy=2365.5,1176.4000244140625
AudioFilterStateVariable filter_string_8; //xy=2365.5,1451.4000244140625
AudioFilterStateVariable filter_string_3; //xy=2366.5,1221.4000244140625
AudioFilterStateVariable filter_string_6; //xy=2367.5,1361.4000244140625
AudioFilterStateVariable filter_string_9; //xy=2367.5,1496.4000244140625
AudioFilterStateVariable filter_string_11; //xy=2368.5,1588.4000244140625
AudioFilterStateVariable filter_string_12; //xy=2368.5,1634.4000244140625
AudioFilterStateVariable filter_string_10; //xy=2369.5,1541.4000244140625
AudioEffectEnvelope      voice4_envelope_filter; //xy=2428.5,2708.4000244140625
AudioMixer4              voice3_mixer;   //xy=2432.5,2289.4000244140625
AudioMixer4              voice4_mixer;   //xy=2433.5,2552.4000244140625
AudioMixer4              voice2_mixer;   //xy=2437.5,2004.4000244140625
AudioEffectEnvelope      voice3_envelope_filter; //xy=2436.5,2395.4000244140625
AudioEffectEnvelope      voice2_envelope_filter; //xy=2452.5,2122.4000244140625
AudioEffectEnvelope      voice1_envelope_filter; //xy=2454.5,1891.4000244140625
AudioMixer4              voice1_mixer;   //xy=2456.5,1734.4000244140625
AudioMixer4              string_mix_3;   //xy=2622.5,1524.4000244140625
AudioMixer4              string_mix_1;   //xy=2625.5,1212.4000244140625
AudioMixer4              string_mix_2;   //xy=2639.5,1341.4000244140625
AudioSynthWaveform       chords_tremolo_lfo; //xy=2704.5,1892.4000244140625
AudioFilterStateVariable voice4_filter;  //xy=2737.5,2556.4000244140625
AudioFilterStateVariable voice2_filter;  //xy=2745.5,2009.4000244140625
AudioFilterStateVariable voice3_filter;  //xy=2744.5,2285.4000244140625
AudioFilterStateVariable voice1_filter;  //xy=2766.5,1741.4000244140625
AudioMixer4              all_string_mix; //xy=2839.5,1341.4000244140625
AudioEffectEnvelope      voice1_envelope; //xy=2922.5,1892.4000244140625
AudioEffectEnvelope      voice3_envelope; //xy=2924.5,2423.4000244140625
AudioEffectEnvelope      voice2_envelope; //xy=2927.5,2143.4000244140625
AudioEffectEnvelope      voice4_envelope; //xy=2931.5,2742.4000244140625
AudioEffectMultiply      voice2_tremolo_mult; //xy=2970.5,2005.4000244140625
AudioEffectMultiply      voice3_tremolo_mult; //xy=2972.5,2285.4000244140625
AudioEffectMultiply      voice4_tremolo_mult; //xy=2975.5,2560.4000244140625
AudioEffectMultiply      voice1_tremolo_mult; //xy=2980.5,1735.4000244140625
AudioEffectWaveshaper    string_waveshape; //xy=3072.5,1159.4000244140625
AudioMixer4              string_waveshaper_mix; //xy=3124.5,1337.4000244140625
AudioMixer4              chord_voice_mixer; //xy=3281.5,1756.4000244140625
AudioFilterStateVariable filter_delay_strings; //xy=3308.5,1125.4000244140625
AudioEffectWaveshaper    chord_waveshape; //xy=3459.5,1652.4000244140625
AudioMixer4              string_delay_mix; //xy=3490.5,1213.4000244140625
AudioMixer4              chord_waveshaper_mix; //xy=3612.5,1737.4000244140625
AudioSynthWaveform       string_tremolo_lfo; //xy=3647.5,1516.4000244140625
AudioMixer4              strings_effect_mix; //xy=3694.5,1394.4000244140625
AudioFilterStateVariable filter_delay_chords; //xy=3704.5,1612.4000244140625
AudioEffectDelay         delay_strings;  //xy=3804.5,1224.4000244140625
AudioSynthWaveform       string_filter_lfo; //xy=3900.5,1563.4000244140625
AudioMixer4              chord_delay_mix; //xy=3904.5,1712.4000244140625
AudioEffectMultiply      string_multiply; //xy=3921.5,1475.4000244140625
AudioMixer4              chords_effect_mix; //xy=4052.5,1860.4000244140625
AudioFilterStateVariable string_filter;  //xy=4119.5,1509.4000244140625
AudioEffectDelay         delay_chords;   //xy=4202.5,1732.4000244140625
AudioInputI2S            i2s1;           //xy=4307.5,1260.4000244140625
AudioMixer4              string_filter_mixer; //xy=4324.5,1510.4000244140625
AudioFilterStateVariable chords_main_filter; //xy=4370.5,1793.4000244140625
AudioMixer4              chords_main_filter_mixer; //xy=4653.5,1808.4000244140625
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
AudioConnection          patchCord144(filter_delay_strings, 0, string_delay_mix, 1);
AudioConnection          patchCord145(filter_delay_strings, 1, string_delay_mix, 2);
AudioConnection          patchCord146(filter_delay_strings, 2, string_delay_mix, 3);
AudioConnection          patchCord147(chord_waveshape, 0, chord_waveshaper_mix, 1);
AudioConnection          patchCord148(string_delay_mix, delay_strings);
AudioConnection          patchCord149(string_delay_mix, 0, strings_effect_mix, 1);
AudioConnection          patchCord150(chord_waveshaper_mix, 0, chord_delay_mix, 0);
AudioConnection          patchCord151(chord_waveshaper_mix, 0, chords_effect_mix, 0);
AudioConnection          patchCord152(string_tremolo_lfo, 0, string_multiply, 1);
AudioConnection          patchCord153(strings_effect_mix, 0, string_multiply, 0);
AudioConnection          patchCord154(filter_delay_chords, 0, chord_delay_mix, 1);
AudioConnection          patchCord155(filter_delay_chords, 1, chord_delay_mix, 2);
AudioConnection          patchCord156(filter_delay_chords, 2, chord_delay_mix, 3);
AudioConnection          patchCord157(delay_strings, 0, filter_delay_strings, 0);
AudioConnection          patchCord158(string_filter_lfo, 0, string_filter, 1);
AudioConnection          patchCord159(chord_delay_mix, delay_chords);
AudioConnection          patchCord160(chord_delay_mix, 0, chords_effect_mix, 1);
AudioConnection          patchCord161(string_multiply, 0, string_filter, 0);
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
AudioAmplifier           string_gain;          
AudioAmplifier           chords_gain;          
AudioMixer4              reverb_mixer;   
AudioEffectPlateReverb   main_reverb;    
AudioMixer4              stereo_l_mixer;        
AudioMixer4              stereo_r_mixer;   
AudioOutputI2S           DAC_out;    
AudioOutputUSB           USB_out;      

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

