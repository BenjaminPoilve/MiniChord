#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioSynthWaveform       string_vibrato_1; //xy=176.0999755859375,748.2000122070312
AudioEffectEnvelope      envelope_string_vibrato_1; //xy=395.0999755859375,749.2000122070312
AudioSynthWaveform       chords_vibrato_lfo; //xy=527.0999755859375,1250.2000122070312
AudioSynthWaveformModulated waveform_string_4; //xy=660.0999755859375,853.2000122070312
AudioSynthWaveformModulated waveform_string_2; //xy=661.0999755859375,785.2000122070312
AudioSynthWaveformModulated waveform_string_5; //xy=661.0999755859375,890.2000122070312
AudioSynthWaveformModulated waveform_string_7; //xy=661.0999755859375,961.2000122070312
AudioSynthWaveformModulated waveform_string_10; //xy=661.0999755859375,1068.2000122070312
AudioSynthWaveformModulated waveform_string_3; //xy=662.0999755859375,819.2000122070312
AudioSynthWaveformModulated waveform_string_12; //xy=661.0999755859375,1135.2000122070312
AudioSynthWaveformModulated waveform_string_6; //xy=662.0999755859375,927.2000122070312
AudioSynthWaveformModulated waveform_string_8; //xy=662.0999755859375,997.2000122070312
AudioSynthWaveformModulated waveform_string_1; //xy=663.0999755859375,750.2000122070312
AudioSynthWaveformModulated waveform_string_9; //xy=663.0999755859375,1033.2000122070312
AudioSynthWaveformModulated waveform_string_11; //xy=663.0999755859375,1103.2000122070312
AudioSynthWaveformDc     filter_dc;      //xy=686.0999755859375,319.20001220703125
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=746.0999755859375,2084.2000122070312
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=753.0999755859375,1819.2000122070312
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=756.0999755859375,1532.2000122070312
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=770.0999755859375,1250.2000122070312
AudioEffectEnvelope      envelope_filter_2; //xy=878.0999755859375,350.20001220703125
AudioEffectEnvelope      envelope_filter_3; //xy=878.0999755859375,380.20001220703125
AudioEffectEnvelope      envelope_filter_5; //xy=878.0999755859375,442.20001220703125
AudioEffectEnvelope      envelope_filter_6; //xy=878.0999755859375,474.20001220703125
AudioEffectEnvelope      envelope_filter_7; //xy=878.0999755859375,505.20001220703125
AudioEffectEnvelope      envelope_filter_1; //xy=879.0999755859375,319.20001220703125
AudioEffectEnvelope      envelope_filter_4; //xy=879.0999755859375,410.20001220703125
AudioEffectEnvelope      envelope_filter_12; //xy=878.0999755859375,663.2000122070312
AudioEffectEnvelope      envelope_filter_9; //xy=879.0999755859375,568.2000122070312
AudioEffectEnvelope      envelope_filter_11; //xy=879.0999755859375,632.2000122070312
AudioEffectEnvelope      envelope_filter_10; //xy=882.0999755859375,601.2000122070312
AudioEffectEnvelope      envelope_filter_8; //xy=883.0999755859375,535.2000122070312
AudioEffectEnvelope      envelope_string_6; //xy=883.0999755859375,925.2000122070312
AudioEffectEnvelope      envelope_string_8; //xy=883.0999755859375,996.2000122070312
AudioEffectEnvelope      envelope_string_10; //xy=883.0999755859375,1064.2000122070312
AudioEffectEnvelope      envelope_string_12; //xy=883.0999755859375,1133.2000122070312
AudioEffectEnvelope      envelope_string_5; //xy=884.0999755859375,890.2000122070312
AudioEffectEnvelope      envelope_string_7; //xy=884.0999755859375,960.2000122070312
AudioEffectEnvelope      envelope_string_9; //xy=884.0999755859375,1030.2000122070312
AudioEffectEnvelope      envelope_string_3; //xy=885.0999755859375,819.2000122070312
AudioEffectEnvelope      envelope_string_4; //xy=886.0999755859375,853.2000122070312
AudioEffectEnvelope      envelope_string_11; //xy=886.0999755859375,1099.2000122070312
AudioEffectEnvelope      envelope_string_1; //xy=888.0999755859375,752.2000122070312
AudioEffectEnvelope      envelope_string_2; //xy=888.0999755859375,785.2000122070312
AudioSynthNoiseWhite     voice3_noise;   //xy=982.0999755859375,1919.2000122070312
AudioSynthWaveformModulated voice3_osc3;    //xy=985.0999755859375,1885.2000122070312
AudioSynthNoiseWhite     voice4_noise;   //xy=985.0999755859375,2196.2000122070312
AudioSynthWaveformModulated voice3_osc2;    //xy=987.0999755859375,1849.2000122070312
AudioSynthWaveformModulated voice4_osc3;    //xy=987.0999755859375,2159.2000122070312
AudioSynthWaveformModulated voice3_osc1;    //xy=991.0999755859375,1811.2000122070312
AudioSynthWaveformModulated voice4_osc2;    //xy=990.0999755859375,2122.2000122070312
AudioSynthWaveformModulated voice4_osc1;    //xy=992.0999755859375,2087.2000122070312
AudioSynthWaveform       chords_filter_LFO; //xy=1005.0999755859375,1442.2000122070312
AudioSynthNoiseWhite     voice2_noise;   //xy=1008.0999755859375,1642.2000122070312
AudioSynthWaveformModulated voice2_osc3;    //xy=1010.0999755859375,1605.2000122070312
AudioSynthWaveformModulated voice2_osc2;    //xy=1012.0999755859375,1569.2000122070312
AudioSynthWaveformModulated voice2_osc1;    //xy=1013.0999755859375,1532.2000122070312
AudioSynthNoiseWhite     voice1_noise;   //xy=1021.0999755859375,1360.2000122070312
AudioSynthWaveformModulated voice1_osc3;    //xy=1026.0999755859375,1324.2000122070312
AudioSynthWaveformModulated voice1_osc1;    //xy=1027.0999755859375,1254.2000122070312
AudioSynthWaveformModulated voice1_osc2;    //xy=1027.0999755859375,1289.2000122070312
AudioFilterStateVariable filter_string_4; //xy=1120.0999755859375,820.2000122070312
AudioFilterStateVariable filter_string_5; //xy=1120.0999755859375,866.2000122070312
AudioFilterStateVariable filter_string_1; //xy=1121.0999755859375,685.2000122070312
AudioFilterStateVariable filter_string_7; //xy=1120.0999755859375,958.2000122070312
AudioFilterStateVariable filter_string_2; //xy=1121.0999755859375,730.2000122070312
AudioFilterStateVariable filter_string_8; //xy=1121.0999755859375,1005.2000122070312
AudioFilterStateVariable filter_string_3; //xy=1122.0999755859375,775.2000122070312
AudioFilterStateVariable filter_string_6; //xy=1123.0999755859375,915.2000122070312
AudioFilterStateVariable filter_string_9; //xy=1123.0999755859375,1050.2000122070312
AudioFilterStateVariable filter_string_11; //xy=1124.0999755859375,1142.2000122070312
AudioFilterStateVariable filter_string_12; //xy=1124.0999755859375,1188.2000122070312
AudioFilterStateVariable filter_string_10; //xy=1125.0999755859375,1095.2000122070312
AudioEffectEnvelope      voice4_envelope_filter; //xy=1184.0999755859375,2262.2000122070312
AudioMixer4              voice3_mixer;   //xy=1188.0999755859375,1843.2000122070312
AudioMixer4              voice4_mixer;   //xy=1189.0999755859375,2106.2000122070312
AudioMixer4              voice2_mixer;   //xy=1193.0999755859375,1558.2000122070312
AudioEffectEnvelope      voice3_envelope_filter; //xy=1192.0999755859375,1949.2000122070312
AudioEffectEnvelope      voice2_envelope_filter; //xy=1208.0999755859375,1676.2000122070312
AudioEffectEnvelope      voice1_envelope_filter; //xy=1210.0999755859375,1445.2000122070312
AudioMixer4              voice1_mixer;   //xy=1212.0999755859375,1288.2000122070312
AudioMixer4              string_mix_3;   //xy=1378.0999755859375,1078.2000122070312
AudioMixer4              string_mix_1;   //xy=1381.0999755859375,766.2000122070312
AudioMixer4              string_mix_2;   //xy=1395.0999755859375,895.2000122070312
AudioSynthWaveform       chords_tremolo_lfo; //xy=1460.0999755859375,1446.2000122070312
AudioFilterStateVariable voice4_filter;  //xy=1493.0999755859375,2110.2000122070312
AudioFilterStateVariable voice2_filter;  //xy=1501.0999755859375,1563.2000122070312
AudioFilterStateVariable voice3_filter;  //xy=1500.0999755859375,1839.2000122070312
AudioFilterStateVariable voice1_filter;  //xy=1522.0999755859375,1295.2000122070312
AudioMixer4              all_string_mix; //xy=1595.0999755859375,895.2000122070312
AudioEffectEnvelope      voice1_envelope; //xy=1678.0999755859375,1446.2000122070312
AudioEffectEnvelope      voice3_envelope; //xy=1680.0999755859375,1977.2000122070312
AudioEffectEnvelope      voice2_envelope; //xy=1683.0999755859375,1697.2000122070312
AudioEffectEnvelope      voice4_envelope; //xy=1687.0999755859375,2296.2000122070312
AudioEffectMultiply      voice2_tremolo_mult; //xy=1726.0999755859375,1559.2000122070312
AudioEffectMultiply      voice3_tremolo_mult; //xy=1728.0999755859375,1839.2000122070312
AudioEffectMultiply      voice4_tremolo_mult; //xy=1731.0999755859375,2114.2000122070312
AudioEffectMultiply      voice1_tremolo_mult; //xy=1736.0999755859375,1289.2000122070312
AudioEffectWaveshaper    string_waveshape; //xy=1828.0999755859375,713.2000122070312
AudioMixer4              string_waveshaper_mix; //xy=1880.099853515625,891.2000122070312
AudioMixer4              chord_voice_mixer; //xy=2037.0999755859375,1310.2000122070312
AudioFilterStateVariable filter_delay_strings; //xy=2064.0999755859375,679.2000122070312
AudioEffectWaveshaper    chord_waveshape; //xy=2215.0999755859375,1206.2000122070312
AudioMixer4              string_delay_mix; //xy=2246.0999755859375,767.2000122070312
AudioMixer4              chord_waveshaper_mix; //xy=2368.10009765625,1291.199951171875
AudioSynthWaveform       string_tremolo_lfo; //xy=2403.0999755859375,1070.199951171875
AudioMixer4              strings_effect_mix; //xy=2450.0999755859375,948.2000122070312
AudioFilterStateVariable filter_delay_chords; //xy=2460.10009765625,1166.199951171875
AudioEffectDelay         delay_strings;  //xy=2560.0999755859375,778.2000122070312
AudioSynthWaveform       string_filter_lfo; //xy=2656.0999755859375,1117.199951171875
AudioMixer4              chord_delay_mix; //xy=2660.10009765625,1266.199951171875
AudioEffectMultiply      string_multiply; //xy=2677.0999755859375,1029.199951171875
AudioMixer4              chords_effect_mix; //xy=2808.10009765625,1414.199951171875
AudioFilterStateVariable string_filter;  //xy=2875.0999755859375,1063.199951171875
AudioEffectDelay         delay_chords;   //xy=2958.10009765625,1286.199951171875
AudioInputI2S            i2s1;           //xy=3063.0999755859375,814.2000122070312
AudioMixer4              string_filter_mixer; //xy=3080.0999755859375,1064.199951171875
AudioFilterStateVariable chords_main_filter; //xy=3126.10009765625,1347.199951171875
AudioMixer4              chords_main_filter_mixer; //xy=3338.10009765625,1362.199951171875
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

