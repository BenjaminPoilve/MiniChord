#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     string_vibrato_dc;            //xy=94.4285888671875,732.999981880188
AudioSynthWaveform       string_vibrato_lfo; //xy=106.35714721679688,665.2142353057861
AudioEffectEnvelope      envelope_string_vibrato_dc;      //xy=260.1428565979004,768.7142505645752
AudioEffectEnvelope      envelope_string_vibrato_lfo; //xy=285.3857727050781,709.0713863372803
AudioSynthWaveformDc     chords_vibrato_dc;            //xy=336.1666259765625,1407.8333740234375
AudioSynthWaveform       chords_vibrato_lfo; //xy=339.0999755859375,1321.5
AudioMixer4              string_vibrato_mixer;         //xy=460.1429100036621,770.1429271697998
AudioEffectEnvelope      voice4_vibrato_dc_envelope;      //xy=545.1000061035156,2221.100149154663
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=549.3856811523438,1891.9286270141602
AudioEffectEnvelope      voice3_vibrato_dc_envelope;      //xy=552.0999755859375,1976.1000061035156
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=558.0999755859375,2155.5
AudioEffectEnvelope      voice2_vibrato_dc_envelope;      //xy=567.0999755859375,1677.1000061035156
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=568.0999755859375,1603.5
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=582.0999755859375,1321.5
AudioEffectEnvelope      voice1_vibrato_dc_envelope;      //xy=582.3015441894531,1406.4601917266846
AudioSynthWaveformModulated waveform_string_4; //xy=646.1000061035156,924.5
AudioSynthWaveformModulated waveform_string_2; //xy=647.1000061035156,856.5
AudioSynthWaveformModulated waveform_string_5; //xy=647.1000061035156,961.5
AudioSynthWaveformModulated waveform_string_7; //xy=647.1000061035156,1032.5
AudioSynthWaveformModulated waveform_string_10; //xy=647.1000061035156,1139.5
AudioSynthWaveformModulated waveform_string_3; //xy=648.1000061035156,890.5
AudioSynthWaveformModulated waveform_string_12; //xy=647.1000061035156,1206.5
AudioSynthWaveformModulated waveform_string_6; //xy=648.1000061035156,998.5
AudioSynthWaveformModulated waveform_string_8; //xy=648.1000061035156,1068.5
AudioSynthWaveformModulated waveform_string_1; //xy=649.1000061035156,821.5
AudioSynthWaveformModulated waveform_string_9; //xy=649.1000061035156,1104.5
AudioSynthWaveformModulated waveform_string_11; //xy=649.1000061035156,1174.5
AudioSynthWaveformDc     filter_dc;      //xy=672.1000061035156,390.5
AudioMixer4              voice3_vibrato_mixer;         //xy=774.4285801478794,1921.5714372907364
AudioMixer4              voice4_vibrato_mixer;         //xy=780.1428909301758,2164.4285278320312
AudioMixer4              voice2_vibrato_mixer;         //xy=805.8571515764509,1630.142865862165
AudioMixer4              voice1_vibrato_mixer;         //xy=813.0000457763672,1341.5715293884277
AudioEffectEnvelope      envelope_filter_2; //xy=864.1000061035156,421.5
AudioEffectEnvelope      envelope_filter_3; //xy=864.1000061035156,451.5
AudioEffectEnvelope      envelope_filter_5; //xy=864.1000061035156,513.5
AudioEffectEnvelope      envelope_filter_6; //xy=864.1000061035156,545.5
AudioEffectEnvelope      envelope_filter_7; //xy=864.1000061035156,576.5
AudioEffectEnvelope      envelope_filter_1; //xy=865.1000061035156,390.5
AudioEffectEnvelope      envelope_filter_4; //xy=865.1000061035156,481.5
AudioEffectEnvelope      envelope_filter_12; //xy=864.1000061035156,734.5
AudioEffectEnvelope      envelope_filter_9; //xy=865.1000061035156,639.5
AudioEffectEnvelope      envelope_filter_11; //xy=865.1000061035156,703.5
AudioEffectEnvelope      envelope_filter_10; //xy=868.1000061035156,672.5
AudioEffectEnvelope      envelope_filter_8; //xy=869.1000061035156,606.5
AudioEffectEnvelope      envelope_string_6; //xy=869.1000061035156,996.5
AudioEffectEnvelope      envelope_string_8; //xy=869.1000061035156,1067.5
AudioEffectEnvelope      envelope_string_10; //xy=869.1000061035156,1135.5
AudioEffectEnvelope      envelope_string_12; //xy=869.1000061035156,1204.5
AudioEffectEnvelope      envelope_string_5; //xy=870.1000061035156,961.5
AudioEffectEnvelope      envelope_string_7; //xy=870.1000061035156,1031.5
AudioEffectEnvelope      envelope_string_9; //xy=870.1000061035156,1101.5
AudioEffectEnvelope      envelope_string_3; //xy=871.1000061035156,890.5
AudioEffectEnvelope      envelope_string_4; //xy=872.1000061035156,924.5
AudioEffectEnvelope      envelope_string_11; //xy=872.1000061035156,1170.5
AudioEffectEnvelope      envelope_string_1; //xy=874.1000061035156,823.5
AudioEffectEnvelope      envelope_string_2; //xy=874.1000061035156,856.5
AudioSynthNoiseWhite     voice3_noise;   //xy=968.1000061035156,1990.5
AudioSynthWaveformModulated voice3_osc3;    //xy=971.1000061035156,1956.5
AudioSynthNoiseWhite     voice4_noise;   //xy=971.1000061035156,2267.5
AudioSynthWaveformModulated voice3_osc2;    //xy=973.1000061035156,1920.5
AudioSynthWaveformModulated voice4_osc3;    //xy=973.1000061035156,2230.5
AudioSynthWaveformModulated voice3_osc1;    //xy=977.1000061035156,1882.5
AudioSynthWaveformModulated voice4_osc2;    //xy=976.1000061035156,2193.5
AudioSynthWaveformModulated voice4_osc1;    //xy=978.1000061035156,2158.5
AudioSynthWaveform       chords_filter_LFO; //xy=991.1000061035156,1513.5
AudioSynthNoiseWhite     voice2_noise;   //xy=994.1000061035156,1713.5
AudioSynthWaveformModulated voice2_osc3;    //xy=996.1000061035156,1676.5
AudioSynthWaveformModulated voice2_osc2;    //xy=998.1000061035156,1640.5
AudioSynthWaveformModulated voice2_osc1;    //xy=999.1000061035156,1603.5
AudioSynthNoiseWhite     voice1_noise;   //xy=1007.1000061035156,1431.5
AudioSynthWaveformModulated voice1_osc3;    //xy=1012.1000061035156,1395.5
AudioSynthWaveformModulated voice1_osc1;    //xy=1013.1000061035156,1325.5
AudioSynthWaveformModulated voice1_osc2;    //xy=1013.1000061035156,1360.5
AudioFilterStateVariable filter_string_4; //xy=1106.1000061035156,891.5
AudioFilterStateVariable filter_string_5; //xy=1106.1000061035156,937.5
AudioFilterStateVariable filter_string_1; //xy=1107.1000061035156,756.5
AudioFilterStateVariable filter_string_7; //xy=1106.1000061035156,1029.5
AudioFilterStateVariable filter_string_2; //xy=1107.1000061035156,801.5
AudioFilterStateVariable filter_string_8; //xy=1107.1000061035156,1076.5
AudioFilterStateVariable filter_string_3; //xy=1108.1000061035156,846.5
AudioFilterStateVariable filter_string_6; //xy=1109.1000061035156,986.5
AudioFilterStateVariable filter_string_9; //xy=1109.1000061035156,1121.5
AudioFilterStateVariable filter_string_11; //xy=1110.1000061035156,1213.5
AudioFilterStateVariable filter_string_12; //xy=1110.1000061035156,1259.5
AudioFilterStateVariable filter_string_10; //xy=1111.1000061035156,1166.5
AudioEffectEnvelope      voice4_envelope_filter; //xy=1170.1000061035156,2333.5
AudioMixer4              voice3_mixer;   //xy=1174.1000061035156,1914.5
AudioMixer4              voice4_mixer;   //xy=1175.1000061035156,2177.5
AudioMixer4              voice2_mixer;   //xy=1179.1000061035156,1629.5
AudioEffectEnvelope      voice3_envelope_filter; //xy=1178.1000061035156,2020.5
AudioEffectEnvelope      voice2_envelope_filter; //xy=1194.1000061035156,1747.5
AudioEffectEnvelope      voice1_envelope_filter; //xy=1196.1000061035156,1516.5
AudioMixer4              voice1_mixer;   //xy=1198.1000061035156,1359.5
AudioMixer4              string_mix_3;   //xy=1364.1000061035156,1149.5
AudioMixer4              string_mix_1;   //xy=1367.1000061035156,837.5
AudioMixer4              string_mix_2;   //xy=1381.1000061035156,966.5
AudioSynthWaveform       chords_tremolo_lfo; //xy=1446.1000061035156,1517.5
AudioFilterStateVariable voice4_filter;  //xy=1479.1000061035156,2181.5
AudioFilterStateVariable voice2_filter;  //xy=1487.1000061035156,1634.5
AudioFilterStateVariable voice3_filter;  //xy=1486.1000061035156,1910.5
AudioFilterStateVariable voice1_filter;  //xy=1508.1000061035156,1366.5
AudioMixer4              all_string_mix; //xy=1581.1000061035156,966.5
AudioEffectEnvelope      voice1_envelope; //xy=1664.1000061035156,1517.5
AudioEffectEnvelope      voice3_envelope; //xy=1666.1000061035156,2048.5
AudioEffectEnvelope      voice2_envelope; //xy=1669.1000061035156,1768.5
AudioEffectEnvelope      voice4_envelope; //xy=1673.1000061035156,2367.5
AudioEffectMultiply      voice2_tremolo_mult; //xy=1712.1000061035156,1630.5
AudioEffectMultiply      voice3_tremolo_mult; //xy=1714.1000061035156,1910.5
AudioEffectMultiply      voice4_tremolo_mult; //xy=1717.1000061035156,2185.5
AudioEffectMultiply      voice1_tremolo_mult; //xy=1722.1000061035156,1360.5
AudioEffectWaveshaper    string_waveshape; //xy=1814.1000061035156,784.5
AudioMixer4              string_waveshaper_mix; //xy=1866.1000061035156,962.5
AudioMixer4              chord_voice_mixer; //xy=2023.1000061035156,1381.5
AudioFilterStateVariable filter_delay_strings; //xy=2050.1000061035156,750.5
AudioEffectWaveshaper    chord_waveshape; //xy=2201.1000061035156,1277.5
AudioMixer4              string_delay_mix; //xy=2232.1000061035156,838.5
AudioMixer4              chord_waveshaper_mix; //xy=2354.1000061035156,1362.5
AudioSynthWaveform       string_tremolo_lfo; //xy=2389.1000061035156,1141.5
AudioMixer4              strings_effect_mix; //xy=2436.1000061035156,1019.5
AudioFilterStateVariable filter_delay_chords; //xy=2446.1000061035156,1237.5
AudioEffectDelay         delay_strings;  //xy=2546.1000061035156,849.5
AudioSynthWaveform       string_filter_lfo; //xy=2642.1000061035156,1188.5
AudioMixer4              chord_delay_mix; //xy=2646.1000061035156,1337.5
AudioEffectMultiply      string_multiply; //xy=2663.1000061035156,1100.5
AudioMixer4              chords_effect_mix; //xy=2794.1000061035156,1485.5
AudioFilterStateVariable string_filter;  //xy=2861.1000061035156,1134.5
AudioEffectDelay         delay_chords;   //xy=2944.1000061035156,1357.5
AudioInputI2S            i2s1;           //xy=3049.1000061035156,885.5
AudioMixer4              string_filter_mixer; //xy=3066.1000061035156,1135.5
AudioFilterStateVariable chords_main_filter; //xy=3112.1000061035156,1418.5
AudioMixer4              chords_main_filter_mixer; //xy=3395.1000061035156,1433.5
AudioConnection          patchCord1(string_vibrato_dc, envelope_string_vibrato_dc);
AudioConnection          patchCord2(string_vibrato_lfo, envelope_string_vibrato_lfo);
AudioConnection          patchCord3(envelope_string_vibrato_dc, 0, string_vibrato_mixer, 1);
AudioConnection          patchCord4(envelope_string_vibrato_lfo, 0, string_vibrato_mixer, 0);
AudioConnection          patchCord5(chords_vibrato_dc, voice1_vibrato_dc_envelope);
AudioConnection          patchCord6(chords_vibrato_dc, voice2_vibrato_dc_envelope);
AudioConnection          patchCord7(chords_vibrato_dc, voice3_vibrato_dc_envelope);
AudioConnection          patchCord8(chords_vibrato_dc, voice4_vibrato_dc_envelope);
AudioConnection          patchCord9(chords_vibrato_lfo, voice1_vibrato_envelope);
AudioConnection          patchCord10(chords_vibrato_lfo, voice2_vibrato_envelope);
AudioConnection          patchCord11(chords_vibrato_lfo, voice3_vibrato_envelope);
AudioConnection          patchCord12(chords_vibrato_lfo, voice4_vibrato_envelope);
AudioConnection          patchCord13(string_vibrato_mixer, 0, waveform_string_1, 0);
AudioConnection          patchCord14(string_vibrato_mixer, 0, waveform_string_2, 0);
AudioConnection          patchCord15(string_vibrato_mixer, 0, waveform_string_3, 0);
AudioConnection          patchCord16(string_vibrato_mixer, 0, waveform_string_4, 0);
AudioConnection          patchCord17(string_vibrato_mixer, 0, waveform_string_5, 0);
AudioConnection          patchCord18(string_vibrato_mixer, 0, waveform_string_6, 0);
AudioConnection          patchCord19(string_vibrato_mixer, 0, waveform_string_7, 0);
AudioConnection          patchCord20(string_vibrato_mixer, 0, waveform_string_8, 0);
AudioConnection          patchCord21(string_vibrato_mixer, 0, waveform_string_9, 0);
AudioConnection          patchCord22(string_vibrato_mixer, 0, waveform_string_10, 0);
AudioConnection          patchCord23(string_vibrato_mixer, 0, waveform_string_11, 0);
AudioConnection          patchCord24(string_vibrato_mixer, 0, waveform_string_12, 0);
AudioConnection          patchCord25(voice4_vibrato_dc_envelope, 0, voice4_vibrato_mixer, 1);
AudioConnection          patchCord26(voice3_vibrato_envelope, 0, voice3_vibrato_mixer, 0);
AudioConnection          patchCord27(voice3_vibrato_dc_envelope, 0, voice3_vibrato_mixer, 1);
AudioConnection          patchCord28(voice4_vibrato_envelope, 0, voice4_vibrato_mixer, 0);
AudioConnection          patchCord29(voice2_vibrato_dc_envelope, 0, voice2_vibrato_mixer, 1);
AudioConnection          patchCord30(voice2_vibrato_envelope, 0, voice2_vibrato_mixer, 0);
AudioConnection          patchCord31(voice1_vibrato_envelope, 0, voice1_vibrato_mixer, 0);
AudioConnection          patchCord32(voice1_vibrato_dc_envelope, 0, voice1_vibrato_mixer, 1);
AudioConnection          patchCord33(waveform_string_4, envelope_string_4);
AudioConnection          patchCord34(waveform_string_2, envelope_string_2);
AudioConnection          patchCord35(waveform_string_5, envelope_string_5);
AudioConnection          patchCord36(waveform_string_7, envelope_string_7);
AudioConnection          patchCord37(waveform_string_10, envelope_string_10);
AudioConnection          patchCord38(waveform_string_3, envelope_string_3);
AudioConnection          patchCord39(waveform_string_12, envelope_string_12);
AudioConnection          patchCord40(waveform_string_6, envelope_string_6);
AudioConnection          patchCord41(waveform_string_8, envelope_string_8);
AudioConnection          patchCord42(waveform_string_1, envelope_string_1);
AudioConnection          patchCord43(waveform_string_9, envelope_string_9);
AudioConnection          patchCord44(waveform_string_11, envelope_string_11);
AudioConnection          patchCord45(filter_dc, envelope_filter_1);
AudioConnection          patchCord46(filter_dc, envelope_filter_2);
AudioConnection          patchCord47(filter_dc, envelope_filter_3);
AudioConnection          patchCord48(filter_dc, envelope_filter_4);
AudioConnection          patchCord49(filter_dc, envelope_filter_5);
AudioConnection          patchCord50(filter_dc, envelope_filter_6);
AudioConnection          patchCord51(filter_dc, envelope_filter_7);
AudioConnection          patchCord52(filter_dc, envelope_filter_8);
AudioConnection          patchCord53(filter_dc, envelope_filter_9);
AudioConnection          patchCord54(filter_dc, envelope_filter_10);
AudioConnection          patchCord55(filter_dc, envelope_filter_11);
AudioConnection          patchCord56(filter_dc, envelope_filter_12);
AudioConnection          patchCord57(voice3_vibrato_mixer, 0, voice3_osc1, 0);
AudioConnection          patchCord58(voice3_vibrato_mixer, 0, voice3_osc2, 0);
AudioConnection          patchCord59(voice3_vibrato_mixer, 0, voice3_osc3, 0);
AudioConnection          patchCord60(voice4_vibrato_mixer, 0, voice4_osc1, 0);
AudioConnection          patchCord61(voice4_vibrato_mixer, 0, voice4_osc2, 0);
AudioConnection          patchCord62(voice4_vibrato_mixer, 0, voice4_osc3, 0);
AudioConnection          patchCord63(voice2_vibrato_mixer, 0, voice2_osc1, 0);
AudioConnection          patchCord64(voice2_vibrato_mixer, 0, voice2_osc2, 0);
AudioConnection          patchCord65(voice2_vibrato_mixer, 0, voice2_osc3, 0);
AudioConnection          patchCord66(voice1_vibrato_mixer, 0, voice1_osc1, 0);
AudioConnection          patchCord67(voice1_vibrato_mixer, 0, voice1_osc2, 0);
AudioConnection          patchCord68(voice1_vibrato_mixer, 0, voice1_osc3, 0);
AudioConnection          patchCord69(envelope_filter_2, 0, filter_string_2, 1);
AudioConnection          patchCord70(envelope_filter_3, 0, filter_string_3, 1);
AudioConnection          patchCord71(envelope_filter_5, 0, filter_string_5, 1);
AudioConnection          patchCord72(envelope_filter_6, 0, filter_string_6, 1);
AudioConnection          patchCord73(envelope_filter_7, 0, filter_string_7, 1);
AudioConnection          patchCord74(envelope_filter_1, 0, filter_string_1, 1);
AudioConnection          patchCord75(envelope_filter_4, 0, filter_string_4, 1);
AudioConnection          patchCord76(envelope_filter_12, 0, filter_string_12, 1);
AudioConnection          patchCord77(envelope_filter_9, 0, filter_string_9, 1);
AudioConnection          patchCord78(envelope_filter_11, 0, filter_string_11, 1);
AudioConnection          patchCord79(envelope_filter_10, 0, filter_string_10, 1);
AudioConnection          patchCord80(envelope_filter_8, 0, filter_string_8, 1);
AudioConnection          patchCord81(envelope_string_6, 0, filter_string_6, 0);
AudioConnection          patchCord82(envelope_string_8, 0, filter_string_8, 0);
AudioConnection          patchCord83(envelope_string_10, 0, filter_string_10, 0);
AudioConnection          patchCord84(envelope_string_12, 0, filter_string_12, 0);
AudioConnection          patchCord85(envelope_string_5, 0, filter_string_5, 0);
AudioConnection          patchCord86(envelope_string_7, 0, filter_string_7, 0);
AudioConnection          patchCord87(envelope_string_9, 0, filter_string_9, 0);
AudioConnection          patchCord88(envelope_string_3, 0, filter_string_3, 0);
AudioConnection          patchCord89(envelope_string_4, 0, filter_string_4, 0);
AudioConnection          patchCord90(envelope_string_11, 0, filter_string_11, 0);
AudioConnection          patchCord91(envelope_string_1, 0, filter_string_1, 0);
AudioConnection          patchCord92(envelope_string_2, 0, filter_string_2, 0);
AudioConnection          patchCord93(voice3_noise, 0, voice3_mixer, 3);
AudioConnection          patchCord94(voice3_osc3, 0, voice3_mixer, 2);
AudioConnection          patchCord95(voice4_noise, 0, voice4_mixer, 3);
AudioConnection          patchCord96(voice3_osc2, 0, voice3_mixer, 1);
AudioConnection          patchCord97(voice4_osc3, 0, voice4_mixer, 2);
AudioConnection          patchCord98(voice3_osc1, 0, voice3_mixer, 0);
AudioConnection          patchCord99(voice4_osc2, 0, voice4_mixer, 1);
AudioConnection          patchCord100(voice4_osc1, 0, voice4_mixer, 0);
AudioConnection          patchCord101(chords_filter_LFO, voice1_envelope_filter);
AudioConnection          patchCord102(chords_filter_LFO, voice2_envelope_filter);
AudioConnection          patchCord103(chords_filter_LFO, voice3_envelope_filter);
AudioConnection          patchCord104(chords_filter_LFO, voice4_envelope_filter);
AudioConnection          patchCord105(voice2_noise, 0, voice2_mixer, 3);
AudioConnection          patchCord106(voice2_osc3, 0, voice2_mixer, 2);
AudioConnection          patchCord107(voice2_osc2, 0, voice2_mixer, 1);
AudioConnection          patchCord108(voice2_osc1, 0, voice2_mixer, 0);
AudioConnection          patchCord109(voice1_noise, 0, voice1_mixer, 3);
AudioConnection          patchCord110(voice1_osc3, 0, voice1_mixer, 2);
AudioConnection          patchCord111(voice1_osc1, 0, voice1_mixer, 0);
AudioConnection          patchCord112(voice1_osc2, 0, voice1_mixer, 1);
AudioConnection          patchCord113(filter_string_4, 0, string_mix_1, 3);
AudioConnection          patchCord114(filter_string_5, 0, string_mix_2, 0);
AudioConnection          patchCord115(filter_string_1, 0, string_mix_1, 0);
AudioConnection          patchCord116(filter_string_7, 0, string_mix_2, 2);
AudioConnection          patchCord117(filter_string_2, 0, string_mix_1, 1);
AudioConnection          patchCord118(filter_string_8, 0, string_mix_2, 3);
AudioConnection          patchCord119(filter_string_3, 0, string_mix_1, 2);
AudioConnection          patchCord120(filter_string_6, 0, string_mix_2, 1);
AudioConnection          patchCord121(filter_string_9, 0, string_mix_3, 0);
AudioConnection          patchCord122(filter_string_11, 0, string_mix_3, 2);
AudioConnection          patchCord123(filter_string_12, 0, string_mix_3, 3);
AudioConnection          patchCord124(filter_string_10, 0, string_mix_3, 1);
AudioConnection          patchCord125(voice4_envelope_filter, 0, voice4_filter, 1);
AudioConnection          patchCord126(voice3_mixer, 0, voice3_filter, 0);
AudioConnection          patchCord127(voice4_mixer, 0, voice4_filter, 0);
AudioConnection          patchCord128(voice2_mixer, 0, voice2_filter, 0);
AudioConnection          patchCord129(voice3_envelope_filter, 0, voice3_filter, 1);
AudioConnection          patchCord130(voice2_envelope_filter, 0, voice2_filter, 1);
AudioConnection          patchCord131(voice1_envelope_filter, 0, voice1_filter, 1);
AudioConnection          patchCord132(voice1_mixer, 0, voice1_filter, 0);
AudioConnection          patchCord133(string_mix_3, 0, all_string_mix, 2);
AudioConnection          patchCord134(string_mix_1, 0, all_string_mix, 0);
AudioConnection          patchCord135(string_mix_2, 0, all_string_mix, 1);
AudioConnection          patchCord136(chords_tremolo_lfo, voice1_envelope);
AudioConnection          patchCord137(chords_tremolo_lfo, voice2_envelope);
AudioConnection          patchCord138(chords_tremolo_lfo, voice3_envelope);
AudioConnection          patchCord139(chords_tremolo_lfo, voice4_envelope);
AudioConnection          patchCord140(voice4_filter, 0, voice4_tremolo_mult, 0);
AudioConnection          patchCord141(voice2_filter, 0, voice2_tremolo_mult, 0);
AudioConnection          patchCord142(voice3_filter, 0, voice3_tremolo_mult, 0);
AudioConnection          patchCord143(voice1_filter, 0, voice1_tremolo_mult, 0);
AudioConnection          patchCord144(all_string_mix, string_waveshape);
AudioConnection          patchCord145(all_string_mix, 0, string_waveshaper_mix, 0);
AudioConnection          patchCord146(voice1_envelope, 0, voice1_tremolo_mult, 1);
AudioConnection          patchCord147(voice3_envelope, 0, voice3_tremolo_mult, 1);
AudioConnection          patchCord148(voice2_envelope, 0, voice2_tremolo_mult, 1);
AudioConnection          patchCord149(voice4_envelope, 0, voice4_tremolo_mult, 1);
AudioConnection          patchCord150(voice2_tremolo_mult, 0, chord_voice_mixer, 1);
AudioConnection          patchCord151(voice3_tremolo_mult, 0, chord_voice_mixer, 2);
AudioConnection          patchCord152(voice4_tremolo_mult, 0, chord_voice_mixer, 3);
AudioConnection          patchCord153(voice1_tremolo_mult, 0, chord_voice_mixer, 0);
AudioConnection          patchCord154(string_waveshape, 0, string_waveshaper_mix, 1);
AudioConnection          patchCord155(string_waveshaper_mix, 0, strings_effect_mix, 0);
AudioConnection          patchCord156(string_waveshaper_mix, 0, string_delay_mix, 0);
AudioConnection          patchCord157(chord_voice_mixer, chord_waveshape);
AudioConnection          patchCord158(chord_voice_mixer, 0, chord_waveshaper_mix, 0);
AudioConnection          patchCord159(filter_delay_strings, 0, string_delay_mix, 1);
AudioConnection          patchCord160(filter_delay_strings, 1, string_delay_mix, 2);
AudioConnection          patchCord161(filter_delay_strings, 2, string_delay_mix, 3);
AudioConnection          patchCord162(chord_waveshape, 0, chord_waveshaper_mix, 1);
AudioConnection          patchCord163(string_delay_mix, delay_strings);
AudioConnection          patchCord164(string_delay_mix, 0, strings_effect_mix, 1);
AudioConnection          patchCord165(chord_waveshaper_mix, 0, chord_delay_mix, 0);
AudioConnection          patchCord166(chord_waveshaper_mix, 0, chords_effect_mix, 0);
AudioConnection          patchCord167(string_tremolo_lfo, 0, string_multiply, 1);
AudioConnection          patchCord168(strings_effect_mix, 0, string_multiply, 0);
AudioConnection          patchCord169(filter_delay_chords, 0, chord_delay_mix, 1);
AudioConnection          patchCord170(filter_delay_chords, 1, chord_delay_mix, 2);
AudioConnection          patchCord171(filter_delay_chords, 2, chord_delay_mix, 3);
AudioConnection          patchCord172(delay_strings, 0, filter_delay_strings, 0);
AudioConnection          patchCord173(string_filter_lfo, 0, string_filter, 1);
AudioConnection          patchCord174(chord_delay_mix, delay_chords);
AudioConnection          patchCord175(chord_delay_mix, 0, chords_effect_mix, 1);
AudioConnection          patchCord176(string_multiply, 0, string_filter, 0);
AudioConnection          patchCord177(chords_effect_mix, 0, chords_main_filter, 0);
AudioConnection          patchCord178(string_filter, 0, string_filter_mixer, 0);
AudioConnection          patchCord179(string_filter, 1, string_filter_mixer, 1);
AudioConnection          patchCord180(string_filter, 2, string_filter_mixer, 2);
AudioConnection          patchCord181(delay_chords, 0, filter_delay_chords, 0);
AudioConnection          patchCord182(chords_main_filter, 0, chords_main_filter_mixer, 0);
AudioConnection          patchCord183(chords_main_filter, 1, chords_main_filter_mixer, 1);
AudioConnection          patchCord184(chords_main_filter, 2, chords_main_filter_mixer, 2);
// GUItool: end automatically generated code

//MANUAL OUTPUT SECTION
#include "effect_platervbstereo.h"
AudioSynthWaveformDc     string_gain; 
AudioEffectMultiply      string_multiplier;  
AudioAmplifier           string_amplifier; 
AudioSynthWaveformDc     chords_gain;        
AudioEffectMultiply      chords_multiplier;  
AudioAmplifier           chords_amplifier; 
AudioMixer4              reverb_mixer;   
AudioEffectPlateReverb   main_reverb;    
AudioMixer4              stereo_l_mixer;        
AudioMixer4              stereo_r_mixer;   
AudioOutputI2S           DAC_out;    
AudioOutputUSB           USB_out;      

AudioConnection          patchCord2000(string_filter_mixer, 0, string_multiplier, 0);
AudioConnection          patchCord2001(string_gain, 0, string_multiplier, 1);
AudioConnection          patchCord2002(string_multiplier, 0, string_amplifier, 0);
AudioConnection          patchCord2003(string_amplifier, 0, stereo_r_mixer, 0);
AudioConnection          patchCord2004(string_amplifier, 0, stereo_l_mixer, 0);
AudioConnection          patchCord2005(string_amplifier, 0, reverb_mixer, 0);

AudioConnection          patchCord2006(chords_main_filter_mixer, 0, chords_multiplier, 0);
AudioConnection          patchCord2007(chords_gain, 0, chords_multiplier, 1);
AudioConnection          patchCord2008(chords_multiplier, 0, chords_amplifier, 0);
AudioConnection          patchCord2009(chords_amplifier, 0, stereo_r_mixer, 1);
AudioConnection          patchCord2010(chords_amplifier, 0, stereo_l_mixer, 1);
AudioConnection          patchCord2011(chords_amplifier, 0, reverb_mixer, 1);

AudioConnection          patchCord2012(reverb_mixer, 0, main_reverb, 0);
AudioConnection          patchCord2013(main_reverb, 0, stereo_r_mixer, 2);
AudioConnection          patchCord2014(main_reverb, 1, stereo_l_mixer, 2);

AudioConnection          patchCord2015(stereo_l_mixer, 0, DAC_out, 1);
AudioConnection          patchCord2016(stereo_r_mixer, 0, DAC_out, 0);

AudioConnection          patchCord2017(stereo_l_mixer, 0, USB_out, 1);
AudioConnection          patchCord2018(stereo_r_mixer, 0, USB_out, 0);

