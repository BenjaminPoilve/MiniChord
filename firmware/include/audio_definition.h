#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     string_vibrato_dc; //xy=228.10000610351562,909.9999923706055
AudioSynthWaveform       string_vibrato_lfo; //xy=240.10000610351562,842.9999923706055
AudioEffectEnvelope      envelope_string_vibrato_dc; //xy=394.1000061035156,945.9999923706055
AudioEffectEnvelope      envelope_string_vibrato_lfo; //xy=419.1000061035156,886.9999923706055
AudioSynthWaveformDc     chords_vibrato_dc; //xy=470.1000061035156,1584.9999923706055
AudioSynthWaveform       chords_vibrato_lfo; //xy=473.1000061035156,1498.9999923706055
AudioMixer4              string_vibrato_mixer; //xy=594.1000061035156,947.9999923706055
AudioEffectEnvelope      voice4_vibrato_dc_envelope; //xy=679.1000061035156,2398.9999923706055
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=683.1000061035156,2068.9999923706055
AudioEffectEnvelope      voice3_vibrato_dc_envelope; //xy=686.1000061035156,2153.9999923706055
AudioSynthWaveformDc     voice3_frequency_dc;            //xy=691.2000122070312,2110.3997802734375
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=692.1000061035156,2332.9999923706055
AudioSynthWaveformDc     voice4_frequency_dc;            //xy=693.2000122070312,2366.3997802734375
AudioEffectEnvelope      voice2_vibrato_dc_envelope; //xy=701.1000061035156,1854.9999923706055
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=702.1000061035156,1780.9999923706055
AudioSynthWaveformDc     voice2_frequency_dc;            //xy=707.2000122070312,1818.39990234375
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=716.1000061035156,1498.9999923706055
AudioEffectEnvelope      voice1_vibrato_dc_envelope; //xy=716.1000061035156,1583.9999923706055
AudioSynthWaveformDc     voice1_frequency_dc;            //xy=721.2000122070312,1538.39990234375
AudioSynthWaveformModulated waveform_string_4; //xy=780.1000061035156,1101.9999923706055
AudioSynthWaveformModulated waveform_string_2; //xy=781.1000061035156,1033.9999923706055
AudioSynthWaveformModulated waveform_string_5; //xy=781.1000061035156,1138.9999923706055
AudioSynthWaveformModulated waveform_string_7; //xy=781.1000061035156,1209.9999923706055
AudioSynthWaveformModulated waveform_string_10; //xy=781.1000061035156,1316.9999923706055
AudioSynthWaveformModulated waveform_string_3; //xy=782.1000061035156,1067.9999923706055
AudioSynthWaveformModulated waveform_string_12; //xy=781.1000061035156,1383.9999923706055
AudioSynthWaveformModulated waveform_string_6; //xy=782.1000061035156,1175.9999923706055
AudioSynthWaveformModulated waveform_string_8; //xy=782.1000061035156,1245.9999923706055
AudioSynthWaveformModulated waveform_string_1; //xy=783.1000061035156,998.9999923706055
AudioSynthWaveformModulated waveform_string_9; //xy=783.1000061035156,1281.9999923706055
AudioSynthWaveformModulated waveform_string_11; //xy=783.1000061035156,1351.9999923706055
AudioSynthWaveformDc     filter_dc;      //xy=806.1000061035156,567.9999923706055
AudioSynthWaveform       waveform_transient_9; //xy=822.1000061035156,410.99999237060547
AudioSynthWaveform       waveform_transient_5; //xy=823.1000061035156,282.99999237060547
AudioSynthWaveform       waveform_transient_6; //xy=823.1000061035156,315.99999237060547
AudioSynthWaveform       waveform_transient_1; //xy=824.1000061035156,149.99999237060547
AudioSynthWaveform       waveform_transient_2; //xy=824.1000061035156,181.99999237060547
AudioSynthWaveform       waveform_transient_3; //xy=824.1000061035156,214.99999237060547
AudioSynthWaveform       waveform_transient_4; //xy=824.1000061035156,249.99999237060547
AudioSynthWaveform       waveform_transient_7; //xy=824.1000061035156,346.99999237060547
AudioSynthWaveform       waveform_transient_8; //xy=825.1000061035156,378.99999237060547
AudioSynthWaveform       waveform_transient_11; //xy=826.1000061035156,469.99999237060547
AudioSynthWaveform       waveform_transient_10; //xy=828.1000061035156,441.99999237060547
AudioSynthWaveform       waveform_transient_12; //xy=829.1000061035156,501.99999237060547
AudioMixer4              voice3_vibrato_mixer; //xy=908.1000061035156,2098.9999923706055
AudioMixer4              voice4_vibrato_mixer; //xy=914.1000061035156,2341.9999923706055
AudioMixer4              voice2_vibrato_mixer; //xy=939.1000061035156,1807.9999923706055
AudioMixer4              voice1_vibrato_mixer; //xy=947.1000061035156,1518.9999923706055
AudioEffectEnvelope      envelope_filter_2; //xy=998.1000061035156,598.9999923706055
AudioEffectEnvelope      envelope_filter_3; //xy=998.1000061035156,628.9999923706055
AudioEffectEnvelope      envelope_filter_5; //xy=998.1000061035156,690.9999923706055
AudioEffectEnvelope      envelope_filter_6; //xy=998.1000061035156,722.9999923706055
AudioEffectEnvelope      envelope_filter_7; //xy=998.1000061035156,753.9999923706055
AudioEffectEnvelope      envelope_filter_1; //xy=999.1000061035156,567.9999923706055
AudioEffectEnvelope      envelope_filter_4; //xy=999.1000061035156,658.9999923706055
AudioEffectEnvelope      envelope_filter_12; //xy=998.1000061035156,911.9999923706055
AudioEffectEnvelope      envelope_filter_9; //xy=999.1000061035156,816.9999923706055
AudioEffectEnvelope      envelope_filter_11; //xy=999.1000061035156,880.9999923706055
AudioEffectEnvelope      envelope_filter_10; //xy=1002.1000061035156,849.9999923706055
AudioEffectEnvelope      envelope_filter_8; //xy=1003.1000061035156,783.9999923706055
AudioEffectEnvelope      envelope_string_6; //xy=1003.1000061035156,1173.9999923706055
AudioEffectEnvelope      envelope_string_8; //xy=1003.1000061035156,1244.9999923706055
AudioEffectEnvelope      envelope_string_10; //xy=1003.1000061035156,1312.9999923706055
AudioEffectEnvelope      envelope_string_12; //xy=1003.1000061035156,1381.9999923706055
AudioEffectEnvelope      envelope_string_5; //xy=1004.1000061035156,1138.9999923706055
AudioEffectEnvelope      envelope_string_7; //xy=1004.1000061035156,1208.9999923706055
AudioEffectEnvelope      envelope_string_9; //xy=1004.1000061035156,1278.9999923706055
AudioEffectEnvelope      envelope_string_3; //xy=1005.1000061035156,1067.9999923706055
AudioEffectEnvelope      envelope_string_4; //xy=1006.1000061035156,1101.9999923706055
AudioEffectEnvelope      envelope_string_11; //xy=1006.1000061035156,1347.9999923706055
AudioEffectEnvelope      envelope_string_1; //xy=1008.1000061035156,1000.9999923706055
AudioEffectEnvelope      envelope_string_2; //xy=1008.1000061035156,1033.9999923706055
AudioEffectEnvelope      envelope_transient_7; //xy=1049.1000061035156,347.99999237060547
AudioEffectEnvelope      envelope_transient_10; //xy=1050.1000061035156,440.99999237060547
AudioEffectEnvelope      envelope_transient_8; //xy=1051.1000061035156,377.99999237060547
AudioEffectEnvelope      envelope_transient_9; //xy=1051.1000061035156,408.99999237060547
AudioEffectEnvelope      envelope_transient_5; //xy=1052.1000061035156,280.99999237060547
AudioEffectEnvelope      envelope_transient_6; //xy=1052.1000061035156,312.99999237060547
AudioEffectEnvelope      envelope_transient_11; //xy=1052.1000061035156,470.99999237060547
AudioEffectEnvelope      envelope_transient_4; //xy=1053.1000061035156,245.99999237060547
AudioEffectEnvelope      envelope_transient_12; //xy=1052.1000061035156,498.99999237060547
AudioEffectEnvelope      envelope_transient_1; //xy=1055.1000061035156,151.99999237060547
AudioEffectEnvelope      envelope_transient_2; //xy=1055.1000061035156,182.99999237060547
AudioEffectEnvelope      envelope_transient_3; //xy=1055.1000061035156,214.99999237060547
AudioSynthNoiseWhite     voice3_noise;   //xy=1102.1000061035156,2167.9999923706055
AudioSynthWaveformModulated voice3_osc3;    //xy=1105.1000061035156,2133.9999923706055
AudioSynthNoiseWhite     voice4_noise;   //xy=1105.1000061035156,2444.9999923706055
AudioSynthWaveformModulated voice3_osc2;    //xy=1107.1000061035156,2097.9999923706055
AudioSynthWaveformModulated voice4_osc3;    //xy=1107.1000061035156,2407.9999923706055
AudioSynthWaveformModulated voice3_osc1;    //xy=1111.1000061035156,2059.9999923706055
AudioSynthWaveformModulated voice4_osc2;    //xy=1110.1000061035156,2370.9999923706055
AudioSynthWaveformModulated voice4_osc1;    //xy=1112.1000061035156,2335.9999923706055
AudioSynthWaveform       chords_filter_LFO; //xy=1125.1000061035156,1690.9999923706055
AudioSynthNoiseWhite     voice2_noise;   //xy=1128.1000061035156,1890.9999923706055
AudioSynthWaveformModulated voice2_osc3;    //xy=1130.1000061035156,1853.9999923706055
AudioSynthWaveformModulated voice2_osc2;    //xy=1132.1000061035156,1817.9999923706055
AudioSynthWaveformModulated voice2_osc1;    //xy=1133.1000061035156,1780.9999923706055
AudioSynthNoiseWhite     voice1_noise;   //xy=1141.1000061035156,1608.9999923706055
AudioSynthWaveformModulated voice1_osc3;    //xy=1146.1000061035156,1572.9999923706055
AudioSynthWaveformModulated voice1_osc1;    //xy=1147.1000061035156,1502.9999923706055
AudioSynthWaveformModulated voice1_osc2;    //xy=1147.1000061035156,1537.9999923706055
AudioFilterStateVariable filter_string_4; //xy=1240.1000061035156,1068.9999923706055
AudioFilterStateVariable filter_string_5; //xy=1240.1000061035156,1114.9999923706055
AudioFilterStateVariable filter_string_1; //xy=1241.1000061035156,933.9999923706055
AudioFilterStateVariable filter_string_7; //xy=1240.1000061035156,1206.9999923706055
AudioFilterStateVariable filter_string_2; //xy=1241.1000061035156,978.9999923706055
AudioFilterStateVariable filter_string_8; //xy=1241.1000061035156,1253.9999923706055
AudioFilterStateVariable filter_string_3; //xy=1242.1000061035156,1023.9999923706055
AudioFilterStateVariable filter_string_6; //xy=1243.1000061035156,1163.9999923706055
AudioFilterStateVariable filter_string_9; //xy=1243.1000061035156,1298.9999923706055
AudioFilterStateVariable filter_string_11; //xy=1244.1000061035156,1390.9999923706055
AudioFilterStateVariable filter_string_12; //xy=1244.1000061035156,1436.9999923706055
AudioFilterStateVariable filter_string_10; //xy=1245.1000061035156,1343.9999923706055
AudioMixer4              transient_mix_2; //xy=1272.1000061035156,272.99999237060547
AudioMixer4              transient_mix_1; //xy=1273.1000061035156,175.99999237060547
AudioMixer4              transient_mix_3; //xy=1278.1000061035156,374.99999237060547
AudioEffectEnvelope      voice4_envelope_filter; //xy=1304.1000061035156,2510.9999923706055
AudioMixer4              voice3_mixer;   //xy=1308.1000061035156,2091.9999923706055
AudioMixer4              voice4_mixer;   //xy=1309.1000061035156,2354.9999923706055
AudioMixer4              voice2_mixer;   //xy=1313.1000061035156,1806.9999923706055
AudioEffectEnvelope      voice3_envelope_filter; //xy=1312.1000061035156,2197.9999923706055
AudioEffectEnvelope      voice2_envelope_filter; //xy=1328.1000061035156,1924.9999923706055
AudioEffectEnvelope      voice1_envelope_filter; //xy=1330.1000061035156,1693.9999923706055
AudioMixer4              voice1_mixer;   //xy=1332.1000061035156,1536.9999923706055
AudioMixer4              transient_full_mix; //xy=1502.1000061035156,244.99999237060547
AudioMixer4              string_mix_3;   //xy=1498.1000061035156,1326.9999923706055
AudioMixer4              string_mix_1;   //xy=1501.1000061035156,1014.9999923706055
AudioMixer4              string_mix_2;   //xy=1515.1000061035156,1143.9999923706055
AudioSynthWaveform       chords_tremolo_lfo; //xy=1580.1000061035156,1694.9999923706055
AudioFilterStateVariable voice4_filter;  //xy=1613.1000061035156,2358.9999923706055
AudioFilterStateVariable voice2_filter;  //xy=1621.1000061035156,1811.9999923706055
AudioFilterStateVariable voice3_filter;  //xy=1620.1000061035156,2087.9999923706055
AudioFilterStateVariable voice1_filter;  //xy=1642.1000061035156,1543.9999923706055
AudioMixer4              all_string_mix; //xy=1715.1000061035156,1143.9999923706055
AudioEffectEnvelope      voice1_envelope; //xy=1798.1000061035156,1694.9999923706055
AudioEffectEnvelope      voice3_envelope; //xy=1800.1000061035156,2225.9999923706055
AudioEffectEnvelope      voice2_envelope; //xy=1803.1000061035156,1945.9999923706055
AudioEffectEnvelope      voice4_envelope; //xy=1807.1000061035156,2544.9999923706055
AudioEffectMultiply      voice2_tremolo_mult; //xy=1846.1000061035156,1807.9999923706055
AudioEffectMultiply      voice3_tremolo_mult; //xy=1848.1000061035156,2087.9999923706055
AudioEffectMultiply      voice4_tremolo_mult; //xy=1851.1000061035156,2362.9999923706055
AudioEffectMultiply      voice1_tremolo_mult; //xy=1856.1000061035156,1537.9999923706055
AudioEffectWaveshaper    string_waveshape; //xy=1948.1000061035156,961.9999923706055
AudioMixer4              string_waveshaper_mix; //xy=2000.1000061035156,1139.9999923706055
AudioMixer4              chord_voice_mixer; //xy=2157.1000061035156,1558.9999923706055
AudioFilterBiquad        formant_1;      //xy=2250,1480
AudioFilterBiquad        formant_2;      //xy=2250,1520
AudioFilterBiquad        formant_3;      //xy=2250,1560
AudioMixer4              formant_mix;    //xy=2380,1540
AudioFilterStateVariable filter_delay_strings; //xy=2184.1000061035156,927.9999923706055
AudioEffectWaveshaper    chord_waveshape; //xy=2335.1000061035156,1454.9999923706055
AudioMixer4              string_delay_mix; //xy=2366.1000061035156,1015.9999923706055
AudioMixer4              chord_waveshaper_mix; //xy=2488.1000061035156,1539.9999923706055
AudioSynthWaveform       string_tremolo_lfo; //xy=2523.1000061035156,1318.9999923706055
AudioMixer4              strings_effect_mix; //xy=2570.1000061035156,1196.9999923706055
AudioFilterStateVariable filter_delay_chords; //xy=2580.1000061035156,1414.9999923706055
AudioEffectDelay         delay_strings;  //xy=2680.1000061035156,1026.9999923706055
AudioSynthWaveform       string_filter_lfo; //xy=2776.1000061035156,1365.9999923706055
AudioMixer4              chord_delay_mix; //xy=2780.1000061035156,1514.9999923706055
AudioEffectMultiply      string_multiply; //xy=2797.1000061035156,1277.9999923706055
AudioMixer4              chords_effect_mix; //xy=2928.1000061035156,1662.9999923706055
AudioFilterStateVariable string_filter;  //xy=2995.1000061035156,1311.9999923706055
AudioEffectDelay         delay_chords;   //xy=3078.1000061035156,1534.9999923706055
AudioInputI2S            i2s1;           //xy=3183.1000061035156,1062.9999923706055
AudioMixer4              string_filter_mixer; //xy=3200.1000061035156,1312.9999923706055
AudioFilterStateVariable chords_main_filter; //xy=3246.1000061035156,1595.9999923706055
AudioMixer4              chords_main_filter_mixer; //xy=3529.1000061035156,1610.9999923706055
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
AudioConnection          patchCord28(voice3_frequency_dc, 0, voice3_vibrato_mixer, 2);
AudioConnection          patchCord29(voice4_vibrato_envelope, 0, voice4_vibrato_mixer, 0);
AudioConnection          patchCord30(voice4_frequency_dc, 0, voice4_vibrato_mixer, 2);
AudioConnection          patchCord31(voice2_vibrato_dc_envelope, 0, voice2_vibrato_mixer, 1);
AudioConnection          patchCord32(voice2_vibrato_envelope, 0, voice2_vibrato_mixer, 0);
AudioConnection          patchCord33(voice2_frequency_dc, 0, voice2_vibrato_mixer, 2);
AudioConnection          patchCord34(voice1_vibrato_envelope, 0, voice1_vibrato_mixer, 0);
AudioConnection          patchCord35(voice1_vibrato_dc_envelope, 0, voice1_vibrato_mixer, 1);
AudioConnection          patchCord36(voice1_frequency_dc, 0, voice1_vibrato_mixer, 2);
AudioConnection          patchCord37(waveform_string_4, envelope_string_4);
AudioConnection          patchCord38(waveform_string_2, envelope_string_2);
AudioConnection          patchCord39(waveform_string_5, envelope_string_5);
AudioConnection          patchCord40(waveform_string_7, envelope_string_7);
AudioConnection          patchCord41(waveform_string_10, envelope_string_10);
AudioConnection          patchCord42(waveform_string_3, envelope_string_3);
AudioConnection          patchCord43(waveform_string_12, envelope_string_12);
AudioConnection          patchCord44(waveform_string_6, envelope_string_6);
AudioConnection          patchCord45(waveform_string_8, envelope_string_8);
AudioConnection          patchCord46(waveform_string_1, envelope_string_1);
AudioConnection          patchCord47(waveform_string_9, envelope_string_9);
AudioConnection          patchCord48(waveform_string_11, envelope_string_11);
AudioConnection          patchCord49(filter_dc, envelope_filter_1);
AudioConnection          patchCord50(filter_dc, envelope_filter_2);
AudioConnection          patchCord51(filter_dc, envelope_filter_3);
AudioConnection          patchCord52(filter_dc, envelope_filter_4);
AudioConnection          patchCord53(filter_dc, envelope_filter_5);
AudioConnection          patchCord54(filter_dc, envelope_filter_6);
AudioConnection          patchCord55(filter_dc, envelope_filter_7);
AudioConnection          patchCord56(filter_dc, envelope_filter_8);
AudioConnection          patchCord57(filter_dc, envelope_filter_9);
AudioConnection          patchCord58(filter_dc, envelope_filter_10);
AudioConnection          patchCord59(filter_dc, envelope_filter_11);
AudioConnection          patchCord60(filter_dc, envelope_filter_12);
AudioConnection          patchCord61(waveform_transient_9, envelope_transient_9);
AudioConnection          patchCord62(waveform_transient_5, envelope_transient_5);
AudioConnection          patchCord63(waveform_transient_6, envelope_transient_6);
AudioConnection          patchCord64(waveform_transient_1, envelope_transient_1);
AudioConnection          patchCord65(waveform_transient_2, envelope_transient_2);
AudioConnection          patchCord66(waveform_transient_3, envelope_transient_3);
AudioConnection          patchCord67(waveform_transient_4, envelope_transient_4);
AudioConnection          patchCord68(waveform_transient_7, envelope_transient_7);
AudioConnection          patchCord69(waveform_transient_8, envelope_transient_8);
AudioConnection          patchCord70(waveform_transient_11, envelope_transient_11);
AudioConnection          patchCord71(waveform_transient_10, envelope_transient_10);
AudioConnection          patchCord72(waveform_transient_12, envelope_transient_12);
AudioConnection          patchCord73(voice3_vibrato_mixer, 0, voice3_osc1, 0);
AudioConnection          patchCord74(voice3_vibrato_mixer, 0, voice3_osc2, 0);
AudioConnection          patchCord75(voice3_vibrato_mixer, 0, voice3_osc3, 0);
AudioConnection          patchCord76(voice4_vibrato_mixer, 0, voice4_osc1, 0);
AudioConnection          patchCord77(voice4_vibrato_mixer, 0, voice4_osc2, 0);
AudioConnection          patchCord78(voice4_vibrato_mixer, 0, voice4_osc3, 0);
AudioConnection          patchCord79(voice2_vibrato_mixer, 0, voice2_osc1, 0);
AudioConnection          patchCord80(voice2_vibrato_mixer, 0, voice2_osc2, 0);
AudioConnection          patchCord81(voice2_vibrato_mixer, 0, voice2_osc3, 0);
AudioConnection          patchCord82(voice1_vibrato_mixer, 0, voice1_osc1, 0);
AudioConnection          patchCord83(voice1_vibrato_mixer, 0, voice1_osc2, 0);
AudioConnection          patchCord84(voice1_vibrato_mixer, 0, voice1_osc3, 0);
AudioConnection          patchCord85(envelope_filter_2, 0, filter_string_2, 1);
AudioConnection          patchCord86(envelope_filter_3, 0, filter_string_3, 1);
AudioConnection          patchCord87(envelope_filter_5, 0, filter_string_5, 1);
AudioConnection          patchCord88(envelope_filter_6, 0, filter_string_6, 1);
AudioConnection          patchCord89(envelope_filter_7, 0, filter_string_7, 1);
AudioConnection          patchCord90(envelope_filter_1, 0, filter_string_1, 1);
AudioConnection          patchCord91(envelope_filter_4, 0, filter_string_4, 1);
AudioConnection          patchCord92(envelope_filter_12, 0, filter_string_12, 1);
AudioConnection          patchCord93(envelope_filter_9, 0, filter_string_9, 1);
AudioConnection          patchCord94(envelope_filter_11, 0, filter_string_11, 1);
AudioConnection          patchCord95(envelope_filter_10, 0, filter_string_10, 1);
AudioConnection          patchCord96(envelope_filter_8, 0, filter_string_8, 1);
AudioConnection          patchCord97(envelope_string_6, 0, filter_string_6, 0);
AudioConnection          patchCord98(envelope_string_8, 0, filter_string_8, 0);
AudioConnection          patchCord99(envelope_string_10, 0, filter_string_10, 0);
AudioConnection          patchCord100(envelope_string_12, 0, filter_string_12, 0);
AudioConnection          patchCord101(envelope_string_5, 0, filter_string_5, 0);
AudioConnection          patchCord102(envelope_string_7, 0, filter_string_7, 0);
AudioConnection          patchCord103(envelope_string_9, 0, filter_string_9, 0);
AudioConnection          patchCord104(envelope_string_3, 0, filter_string_3, 0);
AudioConnection          patchCord105(envelope_string_4, 0, filter_string_4, 0);
AudioConnection          patchCord106(envelope_string_11, 0, filter_string_11, 0);
AudioConnection          patchCord107(envelope_string_1, 0, filter_string_1, 0);
AudioConnection          patchCord108(envelope_string_2, 0, filter_string_2, 0);
AudioConnection          patchCord109(envelope_transient_7, 0, transient_mix_2, 2);
AudioConnection          patchCord110(envelope_transient_10, 0, transient_mix_3, 1);
AudioConnection          patchCord111(envelope_transient_8, 0, transient_mix_2, 3);
AudioConnection          patchCord112(envelope_transient_9, 0, transient_mix_3, 0);
AudioConnection          patchCord113(envelope_transient_5, 0, transient_mix_2, 0);
AudioConnection          patchCord114(envelope_transient_6, 0, transient_mix_2, 1);
AudioConnection          patchCord115(envelope_transient_11, 0, transient_mix_3, 2);
AudioConnection          patchCord116(envelope_transient_4, 0, transient_mix_1, 3);
AudioConnection          patchCord117(envelope_transient_12, 0, transient_mix_3, 3);
AudioConnection          patchCord118(envelope_transient_1, 0, transient_mix_1, 0);
AudioConnection          patchCord119(envelope_transient_2, 0, transient_mix_1, 1);
AudioConnection          patchCord120(envelope_transient_3, 0, transient_mix_1, 2);
AudioConnection          patchCord121(voice3_noise, 0, voice3_mixer, 3);
AudioConnection          patchCord122(voice3_osc3, 0, voice3_mixer, 2);
AudioConnection          patchCord123(voice4_noise, 0, voice4_mixer, 3);
AudioConnection          patchCord124(voice3_osc2, 0, voice3_mixer, 1);
AudioConnection          patchCord125(voice4_osc3, 0, voice4_mixer, 2);
AudioConnection          patchCord126(voice3_osc1, 0, voice3_mixer, 0);
AudioConnection          patchCord127(voice4_osc2, 0, voice4_mixer, 1);
AudioConnection          patchCord128(voice4_osc1, 0, voice4_mixer, 0);
AudioConnection          patchCord129(chords_filter_LFO, voice1_envelope_filter);
AudioConnection          patchCord130(chords_filter_LFO, voice2_envelope_filter);
AudioConnection          patchCord131(chords_filter_LFO, voice3_envelope_filter);
AudioConnection          patchCord132(chords_filter_LFO, voice4_envelope_filter);
AudioConnection          patchCord133(voice2_noise, 0, voice2_mixer, 3);
AudioConnection          patchCord134(voice2_osc3, 0, voice2_mixer, 2);
AudioConnection          patchCord135(voice2_osc2, 0, voice2_mixer, 1);
AudioConnection          patchCord136(voice2_osc1, 0, voice2_mixer, 0);
AudioConnection          patchCord137(voice1_noise, 0, voice1_mixer, 3);
AudioConnection          patchCord138(voice1_osc3, 0, voice1_mixer, 2);
AudioConnection          patchCord139(voice1_osc1, 0, voice1_mixer, 0);
AudioConnection          patchCord140(voice1_osc2, 0, voice1_mixer, 1);
AudioConnection          patchCord141(filter_string_4, 0, string_mix_1, 3);
AudioConnection          patchCord142(filter_string_5, 0, string_mix_2, 0);
AudioConnection          patchCord143(filter_string_1, 0, string_mix_1, 0);
AudioConnection          patchCord144(filter_string_7, 0, string_mix_2, 2);
AudioConnection          patchCord145(filter_string_2, 0, string_mix_1, 1);
AudioConnection          patchCord146(filter_string_8, 0, string_mix_2, 3);
AudioConnection          patchCord147(filter_string_3, 0, string_mix_1, 2);
AudioConnection          patchCord148(filter_string_6, 0, string_mix_2, 1);
AudioConnection          patchCord149(filter_string_9, 0, string_mix_3, 0);
AudioConnection          patchCord150(filter_string_11, 0, string_mix_3, 2);
AudioConnection          patchCord151(filter_string_12, 0, string_mix_3, 3);
AudioConnection          patchCord152(filter_string_10, 0, string_mix_3, 1);
AudioConnection          patchCord153(transient_mix_2, 0, transient_full_mix, 1);
AudioConnection          patchCord154(transient_mix_1, 0, transient_full_mix, 0);
AudioConnection          patchCord155(transient_mix_3, 0, transient_full_mix, 2);
AudioConnection          patchCord156(voice4_envelope_filter, 0, voice4_filter, 1);
AudioConnection          patchCord157(voice3_mixer, 0, voice3_filter, 0);
AudioConnection          patchCord158(voice4_mixer, 0, voice4_filter, 0);
AudioConnection          patchCord159(voice2_mixer, 0, voice2_filter, 0);
AudioConnection          patchCord160(voice3_envelope_filter, 0, voice3_filter, 1);
AudioConnection          patchCord161(voice2_envelope_filter, 0, voice2_filter, 1);
AudioConnection          patchCord162(voice1_envelope_filter, 0, voice1_filter, 1);
AudioConnection          patchCord163(voice1_mixer, 0, voice1_filter, 0);
AudioConnection          patchCord164(transient_full_mix, 0, all_string_mix, 3);
AudioConnection          patchCord165(string_mix_3, 0, all_string_mix, 2);
AudioConnection          patchCord166(string_mix_1, 0, all_string_mix, 0);
AudioConnection          patchCord167(string_mix_2, 0, all_string_mix, 1);
AudioConnection          patchCord168(chords_tremolo_lfo, voice1_envelope);
AudioConnection          patchCord169(chords_tremolo_lfo, voice2_envelope);
AudioConnection          patchCord170(chords_tremolo_lfo, voice3_envelope);
AudioConnection          patchCord171(chords_tremolo_lfo, voice4_envelope);
AudioConnection          patchCord172(voice4_filter, 0, voice4_tremolo_mult, 0);
AudioConnection          patchCord173(voice2_filter, 0, voice2_tremolo_mult, 0);
AudioConnection          patchCord174(voice3_filter, 0, voice3_tremolo_mult, 0);
AudioConnection          patchCord175(voice1_filter, 0, voice1_tremolo_mult, 0);
AudioConnection          patchCord176(all_string_mix, string_waveshape);
AudioConnection          patchCord177(all_string_mix, 0, string_waveshaper_mix, 0);
AudioConnection          patchCord178(voice1_envelope, 0, voice1_tremolo_mult, 1);
AudioConnection          patchCord179(voice3_envelope, 0, voice3_tremolo_mult, 1);
AudioConnection          patchCord180(voice2_envelope, 0, voice2_tremolo_mult, 1);
AudioConnection          patchCord181(voice4_envelope, 0, voice4_tremolo_mult, 1);
AudioConnection          patchCord182(voice2_tremolo_mult, 0, chord_voice_mixer, 1);
AudioConnection          patchCord183(voice3_tremolo_mult, 0, chord_voice_mixer, 2);
AudioConnection          patchCord184(voice4_tremolo_mult, 0, chord_voice_mixer, 3);
AudioConnection          patchCord185(voice1_tremolo_mult, 0, chord_voice_mixer, 0);
AudioConnection          patchCord186(string_waveshape, 0, string_waveshaper_mix, 1);
AudioConnection          patchCord187(string_waveshaper_mix, 0, strings_effect_mix, 0);
AudioConnection          patchCord188(string_waveshaper_mix, 0, string_delay_mix, 0);
AudioConnection          patchCord189(formant_mix, chord_waveshape);
AudioConnection          patchCord190(formant_mix, 0, chord_waveshaper_mix, 0);
// the formants: the chord voices' mix through three band-passes, blended with it dry
AudioConnection          patchCordFormant1(chord_voice_mixer, formant_1);
AudioConnection          patchCordFormant2(chord_voice_mixer, formant_2);
AudioConnection          patchCordFormant3(chord_voice_mixer, formant_3);
AudioConnection          patchCordFormant4(chord_voice_mixer, 0, formant_mix, 0);
AudioConnection          patchCordFormant5(formant_1, 0, formant_mix, 1);
AudioConnection          patchCordFormant6(formant_2, 0, formant_mix, 2);
AudioConnection          patchCordFormant7(formant_3, 0, formant_mix, 3);
AudioConnection          patchCord191(filter_delay_strings, 0, string_delay_mix, 1);
AudioConnection          patchCord192(filter_delay_strings, 1, string_delay_mix, 2);
AudioConnection          patchCord193(filter_delay_strings, 2, string_delay_mix, 3);
AudioConnection          patchCord194(chord_waveshape, 0, chord_waveshaper_mix, 1);
AudioConnection          patchCord195(string_delay_mix, delay_strings);
AudioConnection          patchCord196(string_delay_mix, 0, strings_effect_mix, 1);
AudioConnection          patchCord197(chord_waveshaper_mix, 0, chord_delay_mix, 0);
AudioConnection          patchCord198(chord_waveshaper_mix, 0, chords_effect_mix, 0);
AudioConnection          patchCord199(string_tremolo_lfo, 0, string_multiply, 1);
AudioConnection          patchCord200(strings_effect_mix, 0, string_multiply, 0);
AudioConnection          patchCord201(filter_delay_chords, 0, chord_delay_mix, 1);
AudioConnection          patchCord202(filter_delay_chords, 1, chord_delay_mix, 2);
AudioConnection          patchCord203(filter_delay_chords, 2, chord_delay_mix, 3);
AudioConnection          patchCord204(delay_strings, 0, filter_delay_strings, 0);
AudioConnection          patchCord205(string_filter_lfo, 0, string_filter, 1);
AudioConnection          patchCord206(chord_delay_mix, delay_chords);
AudioConnection          patchCord207(chord_delay_mix, 0, chords_effect_mix, 1);
AudioConnection          patchCord208(string_multiply, 0, string_filter, 0);
AudioConnection          patchCord209(chords_effect_mix, 0, chords_main_filter, 0);
AudioConnection          patchCord210(string_filter, 0, string_filter_mixer, 0);
AudioConnection          patchCord211(string_filter, 1, string_filter_mixer, 1);
AudioConnection          patchCord212(string_filter, 2, string_filter_mixer, 2);
AudioConnection          patchCord213(delay_chords, 0, filter_delay_chords, 0);
AudioConnection          patchCord214(chords_main_filter, 0, chords_main_filter_mixer, 0);
AudioConnection          patchCord215(chords_main_filter, 1, chords_main_filter_mixer, 1);
AudioConnection          patchCord216(chords_main_filter, 2, chords_main_filter_mixer, 2);
// GUItool: end automatically generated code


//MANUAL OUTPUT SECTION
#include "effect_platervbstereo.h"
AudioSynthWaveformDc     string_gain; 
AudioEffectMultiply      string_multiplier;  
AudioAmplifier           string_amplifier; 
AudioSynthWaveformDc     string_l_stereo_gain;        
AudioSynthWaveformDc     string_r_stereo_gain;        
AudioEffectMultiply      string_l_stereo_multiply;  
AudioEffectMultiply      string_r_stereo_multiply;  
AudioSynthWaveformDc     chords_gain;        
AudioEffectMultiply      chords_multiplier;  
AudioAmplifier           chords_amplifier; 
AudioSynthWaveformDc     chords_l_stereo_gain;        
AudioSynthWaveformDc     chords_r_stereo_gain;        
AudioEffectMultiply      chords_l_stereo_multiply;  
AudioEffectMultiply      chords_r_stereo_multiply;  
AudioMixer4              reverb_mixer;   
AudioEffectPlateReverb   main_reverb;    
AudioMixer4              stereo_l_mixer;        
AudioMixer4              stereo_r_mixer;   
AudioOutputI2S           DAC_out;
#ifdef AUDIO_INTERFACE
AudioOutputUSB           USB_out;
#endif

AudioConnection          patchCord2000(string_filter_mixer, 0, string_multiplier, 0);
AudioConnection          patchCord2001(string_gain, 0, string_multiplier, 1);
AudioConnection          patchCord2002(string_multiplier, 0, string_amplifier, 0);
AudioConnection          patchCord2003(string_amplifier, 0, string_l_stereo_multiply, 0);
AudioConnection          patchCord2004(string_l_stereo_gain, 0, string_l_stereo_multiply, 1);
AudioConnection          patchCord2005(string_amplifier, 0, string_r_stereo_multiply, 0);
AudioConnection          patchCord2006(string_r_stereo_gain, 0, string_r_stereo_multiply, 1);
AudioConnection          patchCord2007(string_r_stereo_multiply, 0, stereo_r_mixer, 0);
AudioConnection          patchCord2008(string_l_stereo_multiply, 0, stereo_l_mixer, 0);
AudioConnection          patchCord2009(string_amplifier, 0, reverb_mixer, 0);

AudioConnection          patchCord2010(chords_main_filter_mixer, 0, chords_multiplier, 0);
AudioConnection          patchCord2011(chords_gain, 0, chords_multiplier, 1);
AudioConnection          patchCord2012(chords_multiplier, 0, chords_amplifier, 0);
AudioConnection          patchCord2013(chords_amplifier, 0, chords_l_stereo_multiply, 0);
AudioConnection          patchCord2014(chords_l_stereo_gain, 0, chords_l_stereo_multiply, 1);
AudioConnection          patchCord2015(chords_amplifier, 0, chords_r_stereo_multiply, 0);
AudioConnection          patchCord2016(chords_r_stereo_gain, 0, chords_r_stereo_multiply, 1);
AudioConnection          patchCord2017(chords_r_stereo_multiply, 0, stereo_r_mixer, 1);
AudioConnection          patchCord2018(chords_l_stereo_multiply, 0, stereo_l_mixer, 1);
AudioConnection          patchCord2019(chords_amplifier, 0, reverb_mixer, 1);

AudioConnection          patchCord2020(reverb_mixer, 0, main_reverb, 0);
AudioConnection          patchCord2021(main_reverb, 0, stereo_r_mixer, 2);
AudioConnection          patchCord2022(main_reverb, 1, stereo_l_mixer, 2);

AudioConnection          patchCord2023(stereo_l_mixer, 0, DAC_out, 1);
AudioConnection          patchCord2024(stereo_r_mixer, 0, DAC_out, 0);

// USB audio recording tap; absent when the USB type has no audio interfaces.
// The headphone/speaker path (patchCord2023/2024 into DAC_out) is unaffected.
#ifdef AUDIO_INTERFACE
AudioConnection          patchCord2025(stereo_l_mixer, 0, USB_out, 1);
AudioConnection          patchCord2026(stereo_r_mixer, 0, USB_out, 0);
#endif
