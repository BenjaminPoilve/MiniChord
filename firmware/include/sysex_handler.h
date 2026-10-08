void apply_audio_parameter(int adress, int value) {
    switch(adress){
      case 20:
        bank_led_hue=value; set_led_color(bank_led_hue, 1.0, 1-led_attenuation);
        break;
      case 32:
        led_attenuation=value/100.0; set_led_color(bank_led_hue, 1.0, 1-led_attenuation);
        break;
      case 21:
        retrigger_chord=value;
        break;
      case 22:
        change_held_strings=value;
        break;
      case 23:
        note_slash_level=value;
        break;
      case 30:
        transpose_semitones=value;midi_base_note_transposed=midi_base_note+transpose_semitones;
        break;
      case 106:
        chord_channel=max(value,1);
        break;
      case 107:
        harp_channel=max(value,1);
        break;
      case 108:
        harp_port=1-value;
        break;
      case 31:
        flat_button_modifier=value;
        break;
      case 33:
        barry_harris_mode=value;
        break;
      case 34:
        chord_frame_shift=value;
        break;
      case 35:
        key_signature_selection=constrain(value,0,11);
        break;
      case 36:
        scalar_harp_selection=value; for (int i=0;i<12;i++){ current_harp_notes[i]=calculate_note_harp(i,slash_chord,sharp_active); }
        break;
      case 37:
        chord_inversion=value; for (int i = 0; i < 7; i++) { current_chord_notes[i]=calculate_note_chord(i,slash_chord,sharp_active); } for (int i = 0; i < 4; i++) { if (chord_envelope_array[i]->isActive()) { set_chord_voice_frequency(i, current_chord_notes[i]); } }
        break;
      case 38:
        chord_spacing=value; for (int i = 0; i < 7; i++) { current_chord_notes[i]=calculate_note_chord(i,slash_chord,sharp_active); } for (int i = 0; i < 4; i++) { if (chord_envelope_array[i]->isActive()) { set_chord_voice_frequency(i, current_chord_notes[i]); } }
        break;
      case 39:
        alt_chord_layout=value;
        break;
      case 202:
        
        break;
      case 203:
        
        break;
      case 204:
        
        break;
      case 205:
        
        break;
      case 206:
        
        break;
      case 207:
        
        break;
      case 208:
        
        break;
      case 24:
        main_reverb.size(value/100.0);
        break;
      case 25:
        main_reverb.hidamp(value/100.0);
        break;
      case 26:
        main_reverb.lodamp(value/100.0);
        break;
      case 27:
        main_reverb.lowpass(value/100.0);
        break;
      case 28:
        main_reverb.diffusion(value/100.0);
        break;
      case 29:
        pan=value/100.0;apply_audio_parameter(85, current_sysex_parameters[85]);apply_audio_parameter(184, current_sysex_parameters[184]);
        break;
      case 10:
        chord_pot.set_alternate(knob_target(value));
        break;
      case 11:
        chord_pot.set_alternate_range(value);
        break;
      case 12:
        harp_pot.set_alternate(knob_target(value));
        break;
      case 13:
        harp_pot.set_alternate_range(value);
        break;
      case 14:
        mod_pot.set_main(knob_target(value));
        break;
      case 15:
        mod_pot.set_main_range(value);
        break;
      case 16:
        mod_pot.set_alternate(knob_target(value));
        break;
      case 17:
        mod_pot.set_alternate_range(value);
        break;
      case 249:
        hover_set_target(constrain(value,0,parameter_size-1));
        break;
      case 250:
        hover_value=value; hover_reapply=true;
        break;
      case 251:
        hover_reach=value ? constrain(value,3,10) : 7;
        break;
      case 4:
        chord_pot.set_alternate_default(value);chord_pot.force_update();
        break;
      case 5:
        harp_pot.set_alternate_default(value);harp_pot.force_update();
        break;
      case 6:
        mod_pot.set_alternate_default(value);mod_pot.force_update();
        break;
      case 7:
        current_sysex_parameters[7]=version_ID;
        break;
      case 2:
        string_gain.amplitude(value/100.0,100);  harp_attack_velocity=value/100.0*127;
        break;
      case 99:
        for (int i=0;i<12;i++){
          harp_octave_change=value; current_harp_notes[i]=calculate_note_harp(i,slash_chord,sharp_active);
        }
        break;
      case 40:
        for (int i=0;i<12;i++){
          harp_shuffling_selection=constrain(value,0,6); current_harp_notes[i]=calculate_note_harp(i,slash_chord,sharp_active);
        }
        break;
      case 98:
        chromatic_harp_mode=value;
        break;
      case 41:
        for (int i=0;i<12;i++){
          string_waveform_array[i]->amplitude(value/100.0);
        }
        break;
      case 42:
        for (int i=0;i<12;i++){
          string_waveform_array[i]->begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 43:
        for (int i=0;i<12;i++){
          string_enveloppe_array[i]->attack(value);
        }
        break;
      case 44:
        for (int i=0;i<12;i++){
          string_enveloppe_array[i]->hold(value);
        }
        break;
      case 45:
        for (int i=0;i<12;i++){
          string_enveloppe_array[i]->decay(value);
        }
        break;
      case 46:
        for (int i=0;i<12;i++){
              string_enveloppe_array[i]->sustain(value/100.0);
        }
        break;
      case 47:
        for (int i=0;i<12;i++){
          string_enveloppe_array[i]->release(value);
        }
        break;
      case 48:
        for (int i=0;i<12;i++){
          string_enveloppe_array[i]->releaseNoteOn(value);
        }
        break;
      case 49:
        string_filter_base_freq=value;
        break;
      case 50:
        string_filter_keytrack=value/100.0;
        break;
      case 51:
        for (int i=0;i<12;i++){
          string_filter_array[i]->resonance(value/100.0);
        }
        break;
      case 52:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->attack(value);
        }
        break;
      case 53:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->hold(value);
        }
        break;
      case 54:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->decay(value);
        }
        break;
      case 55:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->sustain(value/100.0);
        }
        break;
      case 56:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->release(value);
        }
        break;
      case 57:
        for (int i=0;i<12;i++){
          string_enveloppe_filter_array[i]->releaseNoteOn(value);
        }
        break;
      case 58:
        for (int i=0;i<12;i++){
          string_filter_array[i]->octaveControl(value/100.0);
        }
        break;
      case 100:
        for (int i=0;i<12;i++){
          string_transient_waveform_array[i]->begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 101:
        for (int i=0;i<12;i++){
          string_transient_waveform_array[i]->amplitude(value/100.0);
        }
        break;
      case 102:
        for (int i=0;i<12;i++){
          string_transient_envelope_array[i]->attack(value);
        }
        break;
      case 103:
        for (int i=0;i<12;i++){
          string_transient_envelope_array[i]->hold(value);
        }
        break;
      case 104:
        for (int i=0;i<12;i++){
          string_transient_envelope_array[i]->decay(value);string_transient_envelope_array[i]->release(value);
        }
        break;
      case 105:
        transient_note_level=value;
        break;
      case 59:
        string_tremolo_lfo.begin(waveform_array[constrain(value,0,11)]);
        break;
      case 60:
        string_tremolo_lfo.frequency(value/100.0);
        break;
      case 61:
        string_tremolo_lfo.amplitude(0.01+value/100.0);string_tremolo_lfo.offset(1-value/100.0);
        break;
      case 62:
        string_vibrato_lfo.begin(waveform_array[constrain(value,0,11)]);
        break;
      case 63:
        string_vibrato_lfo.frequency(value/100.0);
        break;
      case 64:
        string_vibrato_lfo.amplitude(0.01+value/100.0);
        break;
      case 65:
        envelope_string_vibrato_lfo.attack(value);
        break;
      case 66:
        envelope_string_vibrato_lfo.hold(value);
        break;
      case 67:
        envelope_string_vibrato_lfo.decay(value);
        break;
      case 68:
        envelope_string_vibrato_lfo.sustain(value/100.0);
        break;
      case 69:
        envelope_string_vibrato_lfo.release(value);
        break;
      case 70:
        envelope_string_vibrato_lfo.releaseNoteOn(value);
        break;
      case 71:
        string_vibrato_dc.amplitude(value/100.0-1);
        break;
      case 72:
        envelope_string_vibrato_dc.attack(value);
        break;
      case 73:
        envelope_string_vibrato_dc.hold(value);
        break;
      case 74:
        envelope_string_vibrato_dc.decay(value);
        break;
      case 75:
        envelope_string_vibrato_dc.releaseNoteOn(value);
        break;
      case 76:
        for (int i=0;i<12;i++){
          string_waveform_array[i]->frequencyModulation(value/100.0);
        }
        break;
      case 77:
        delay_strings.delay(0,value);
        break;
      case 78:
        filter_delay_strings.frequency(value);
        break;
      case 79:
        filter_delay_strings.resonance(value/100.0);
        break;
      case 80:
        string_delay_mix.gain(1,value/100.0);
        break;
      case 81:
        string_delay_mix.gain(2,value/100.0);
        break;
      case 82:
        string_delay_mix.gain(3,value/100.0);
        break;
      case 83:
        strings_effect_mix.gain(0,value/100.0);
        break;
      case 84:
        strings_effect_mix.gain(1,value/100.0);
        break;
      case 85:
        reverb_mixer.gain(0,value/100.0);string_r_stereo_gain.amplitude((1-reverb_dry_proportion*value/100.0)*pan,100);string_l_stereo_gain.amplitude(1-reverb_dry_proportion*value/100.0,100);
        break;
      case 86:
        string_waveshaper_mix.gain(0,1-value/100.0);string_waveshaper_mix.gain(1,value/100.0);
        break;
      case 87:
        ws_sin_param=value;calculate_ws_array(); string_waveshape.shape(wave_shape,257);
        break;
      case 88:
        string_filter.frequency(value);
        break;
      case 89:
        string_filter.resonance(value/100.0);
        break;
      case 90:
        string_filter_mixer.gain(0,value/100.0);
        break;
      case 91:
        string_filter_mixer.gain(1,value/100.0);
        break;
      case 92:
        string_filter_mixer.gain(2,value/100.0);
        break;
      case 93:
        string_filter_lfo.begin(waveform_array[constrain(value,0,11)]);
        break;
      case 94:
        string_filter_lfo.frequency(value/100.0);
        break;
      case 95:
        string_filter_lfo.amplitude(value/100.0);
        break;
      case 96:
        string_filter.octaveControl(value/100.0);
        break;
      case 97:
        string_amplifier.gain(value/100.0);
        break;
      case 3:
        chords_gain.amplitude(value/100.0,100); chord_attack_velocity=value/100.0*127;
        break;
      case 120:
        for (int i=0;i<7;i++){
          chord_shuffling_selection=constrain(value,0,5); current_chord_notes[i]=calculate_note_chord(i,slash_chord,sharp_active);
        }
        break;
      case 198:
        for (int i=0;i<4;i++){
          chord_octave_change=value; current_chord_notes[i]=calculate_note_chord(i,slash_chord,sharp_active);
        }
        break;
      case 199:
        glide_length=value;
        break;
      case 121:
        for (int i=0;i<4;i++){
          chord_osc_1_array[i]->amplitude(value/100.0);
        }
        break;
      case 122:
        for (int i=0;i<4;i++){
          chord_osc_1_array[i]->begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 123:
        osc_1_freq_multiplier=value/100.0;
        break;
      case 124:
        for (int i=0;i<4;i++){
          chord_osc_2_array[i]->amplitude(value/100.0);
        }
        break;
      case 125:
        for (int i=0;i<4;i++){
          chord_osc_2_array[i]->begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 126:
        osc_2_freq_multiplier=value/100.0;
        break;
      case 127:
        for (int i=0;i<4;i++){
          chord_osc_3_array[i]->amplitude(value/100.0);
        }
        break;
      case 128:
        for (int i=0;i<4;i++){
          chord_osc_3_array[i]->begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 129:
        osc_3_freq_multiplier=value/100.0;
        break;
      case 130:
        for (int i=0;i<4;i++){
          chord_voice_mixer_array[i]->gain(3,value/100.0);
        }
        break;
      case 131:
        chord_voice_mixer.gain(0,value/100.0);
        break;
      case 132:
        chord_voice_mixer.gain(1,value/100.0);
        break;
      case 133:
        chord_voice_mixer.gain(2,value/100.0);
        break;
      case 134:
        chord_voice_mixer.gain(3,value/100.0);
        break;
      case 135:
        inter_string_delay=value*1000;
        break;
      case 136:
        random_delay=value*1000;
        break;
      case 137:
        for (int i=0;i<4;i++){
          chord_envelope_array[i]->attack(value);
        }
        break;
      case 138:
        for (int i=0;i<4;i++){
          chord_envelope_array[i]->hold(value);
        }
        break;
      case 139:
        for (int i=0;i<4;i++){
          chord_envelope_array[i]->decay(value);
        }
        break;
      case 140:
        for (int i=0;i<4;i++){
              chord_envelope_array[i]->sustain(value/100.0);
        }
        break;
      case 141:
        for (int i=0;i<4;i++){
          chord_envelope_array[i]->release(value);
        }
        break;
      case 142:
        for (int i=0;i<4;i++){
          chord_envelope_array[i]->releaseNoteOn(value); chord_retrigger_release=value;
        }
        break;
      case 143:
        chord_filter_base_freq=value; refresh_chord_filter();
        break;
      case 144:
        chord_filter_keytrack=value/100.0; refresh_chord_filter();
        break;
      case 145:
        for (int i=0;i<4;i++){
          chord_voice_filter_array[i]->resonance(value/100.0);
        }
        break;
      case 146:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->attack(value);
        }
        break;
      case 147:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->hold(value);
        }
        break;
      case 148:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->decay(value);
        }
        break;
      case 149:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->sustain(value/100.0);
        }
        break;
      case 150:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->release(value);
        }
        break;
      case 151:
        for (int i=0;i<4;i++){
          chord_envelope_filter_array[i]->releaseNoteOn(value);
        }
        break;
      case 152:
        chords_filter_LFO.begin(waveform_array[constrain(value,0,11)]);
        break;
      case 153:
        chords_filter_LFO.frequency(value/100.0);
        break;
      case 154:
        chords_filter_LFO.amplitude(0.01+value/100.0);chords_filter_LFO.offset(1-value/100.0);
        break;
      case 155:
        for (int i=0;i<4;i++){
          chord_voice_filter_array[i]->octaveControl(value/100.0);
        }
        break;
      case 156:
        for (int i=0;i<4;i++){
          chords_tremolo_lfo.begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 157:
        chord_tremolo_base_freq=value/100.0;
        break;
      case 158:
        chord_tremolo_keytrack=value/100.0;
        break;
      case 159:
        for (int i=0;i<4;i++){
          chords_tremolo_lfo.amplitude(0.01+value/100.0);chords_tremolo_lfo.offset(1-value/100.0);
        }
        break;
      case 160:
        for (int i=0;i<4;i++){
          chords_vibrato_lfo.begin(waveform_array[constrain(value,0,11)]);
        }
        break;
      case 161:
        chord_vibrato_base_freq=value/100.0;
        break;
      case 162:
        chord_vibrato_keytrack=value/100.0;
        break;
      case 163:
        chords_vibrato_lfo.amplitude(0.01+value/100.0);
        break;
      case 164:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->attack(value);
        }
        break;
      case 165:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->hold(value);
        }
        break;
      case 166:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->decay(value);
        }
        break;
      case 167:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->sustain(value/100.0);
        }
        break;
      case 168:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->release(value);
        }
        break;
      case 169:
        for (int i=0;i<4;i++){
          chord_vibrato_envelope_array[i]->releaseNoteOn(value);
        }
        break;
      case 170:
        chords_vibrato_dc.amplitude(value/100.0-1);
        break;
      case 171:
        for (int i=0;i<4;i++){
          chord_vibrato_dc_envelope_array[i]->attack(value);
        }
        break;
      case 172:
        for (int i=0;i<4;i++){
          chord_vibrato_dc_envelope_array[i]->hold(value);
        }
        break;
      case 173:
        for (int i=0;i<4;i++){
          chord_vibrato_dc_envelope_array[i]->decay(value);
        }
        break;
      case 174:
        for (int i=0;i<4;i++){
          chord_vibrato_dc_envelope_array[i]->releaseNoteOn(value);
        }
        break;
      case 175:
        for (int i=0;i<4;i++){
          chord_vibrato_mixer_array[i]->gain(0,value/100.0/4.0);chord_vibrato_mixer_array[i]->gain(1,value/100.0/4.0);
        }
        break;
      case 176:
        delay_chords.delay(0,value);
        break;
      case 177:
        filter_delay_chords.frequency(value);
        break;
      case 178:
        filter_delay_chords.resonance(value/100.0);
        break;
      case 179:
        chord_delay_mix.gain(1,value/100.0);
        break;
      case 180:
        chord_delay_mix.gain(2,value/100.0);
        break;
      case 181:
        chord_delay_mix.gain(3,value/100.0);
        break;
      case 182:
        chords_effect_mix.gain(0,value/100.0);
        break;
      case 183:
        chords_effect_mix.gain(1,value/100.0);
        break;
      case 184:
        reverb_mixer.gain(1,value/100.0);chords_r_stereo_gain.amplitude(1.0-reverb_dry_proportion*value/100.0,100);chords_l_stereo_gain.amplitude((1.0-reverb_dry_proportion*value/100.0)*pan,100);
        break;
      case 185:
        chord_waveshaper_mix.gain(0,1-value/100.0);chord_waveshaper_mix.gain(1,value/100.0);
        break;
      case 186:
        ws_sin_param=value;calculate_ws_array(); chord_waveshape.shape(wave_shape,257);
        break;
      case 187:
        rythm_bpm=constrain(value,30,300);recalculate_timer();
        break;
      case 188:
        rythm_loop_length=constrain(value,1,16);
        break;
      case 189:
        rythm_limit_change_to_every=constrain(value,1,8);
        break;
      case 190:
        shuffle=value/100.0;recalculate_timer();
        break;
      case 191:
        note_pushed_duration=value;
        break;
      case 220:
        rythm_pattern[0]=value;
        break;
      case 221:
        rythm_pattern[1]=value;
        break;
      case 222:
        rythm_pattern[2]=value;
        break;
      case 223:
        rythm_pattern[3]=value;
        break;
      case 224:
        rythm_pattern[4]=value;
        break;
      case 225:
        rythm_pattern[5]=value;
        break;
      case 226:
        rythm_pattern[6]=value;
        break;
      case 227:
        rythm_pattern[7]=value;
        break;
      case 228:
        rythm_pattern[8]=value;
        break;
      case 229:
        rythm_pattern[9]=value;
        break;
      case 230:
        rythm_pattern[10]=value;
        break;
      case 231:
        rythm_pattern[11]=value;
        break;
      case 232:
        rythm_pattern[12]=value;
        break;
      case 233:
        rythm_pattern[13]=value;
        break;
      case 234:
        rythm_pattern[14]=value;
        break;
      case 235:
        rythm_pattern[15]=value;
        break;
      case 192:
        chords_main_filter.frequency(value);
        break;
      case 193:
        chords_main_filter.resonance(value/100.0);
        break;
      case 194:
        chords_main_filter_mixer.gain(0,value/100.0);
        break;
      case 195:
        chords_main_filter_mixer.gain(1,value/100.0);
        break;
      case 196:
        chords_main_filter_mixer.gain(2,value/100.0);
        break;
      case 197:
        chords_amplifier.gain(value/100.0);
        break;
  }
}

// the range parameters.json declares for each parameter, as stored (floats in hundredths)
bool parameter_range(int adress, int16_t &lo, int16_t &hi) {
    switch(adress){
      case 20: lo=0; hi=360; return true;
      case 32: lo=0; hi=100; return true;
      case 21: lo=0; hi=1; return true;
      case 22: lo=0; hi=1; return true;
      case 23: lo=0; hi=2; return true;
      case 30: lo=0; hi=12; return true;
      case 106: lo=1; hi=16; return true;
      case 107: lo=1; hi=16; return true;
      case 108: lo=0; hi=1; return true;
      case 31: lo=0; hi=1; return true;
      case 33: lo=0; hi=1; return true;
      case 34: lo=0; hi=6; return true;
      case 35: lo=0; hi=11; return true;
      case 36: lo=0; hi=9; return true;
      case 37: lo=0; hi=3; return true;
      case 38: lo=0; hi=4; return true;
      case 39: lo=0; hi=1; return true;
      case 202: lo=0; hi=18; return true;
      case 203: lo=0; hi=18; return true;
      case 204: lo=0; hi=18; return true;
      case 205: lo=0; hi=18; return true;
      case 206: lo=0; hi=18; return true;
      case 207: lo=0; hi=18; return true;
      case 208: lo=0; hi=18; return true;
      case 24: lo=0; hi=100; return true;
      case 25: lo=0; hi=100; return true;
      case 26: lo=0; hi=100; return true;
      case 27: lo=0; hi=100; return true;
      case 28: lo=0; hi=100; return true;
      case 29: lo=0; hi=100; return true;
      case 10: lo=21; hi=511; return true;
      case 11: lo=0; hi=100; return true;
      case 12: lo=21; hi=511; return true;
      case 13: lo=0; hi=100; return true;
      case 14: lo=21; hi=511; return true;
      case 15: lo=0; hi=100; return true;
      case 16: lo=21; hi=511; return true;
      case 17: lo=0; hi=100; return true;
      case 249: lo=0; hi=511; return true;
      case 250: lo=0; hi=4095; return true;
      case 251: lo=3; hi=10; return true;
      case 4: lo=0; hi=1024; return true;
      case 5: lo=0; hi=1024; return true;
      case 6: lo=0; hi=1024; return true;
      case 7: lo=0; hi=1000; return true;
      case 2: lo=0; hi=100; return true;
      case 99: lo=0; hi=4; return true;
      case 40: lo=0; hi=6; return true;
      case 98: lo=0; hi=1; return true;
      case 41: lo=0; hi=100; return true;
      case 42: lo=0; hi=11; return true;
      case 43: lo=0; hi=5000; return true;
      case 44: lo=0; hi=5000; return true;
      case 45: lo=0; hi=5000; return true;
      case 46: lo=0; hi=100; return true;
      case 47: lo=0; hi=5000; return true;
      case 48: lo=0; hi=10; return true;
      case 49: lo=0; hi=2000; return true;
      case 50: lo=0; hi=300; return true;
      case 51: lo=70; hi=500; return true;
      case 52: lo=0; hi=5000; return true;
      case 53: lo=0; hi=5000; return true;
      case 54: lo=0; hi=5000; return true;
      case 55: lo=0; hi=100; return true;
      case 56: lo=0; hi=5000; return true;
      case 57: lo=0; hi=100; return true;
      case 58: lo=0; hi=500; return true;
      case 100: lo=0; hi=11; return true;
      case 101: lo=0; hi=100; return true;
      case 102: lo=0; hi=5000; return true;
      case 103: lo=0; hi=5000; return true;
      case 104: lo=0; hi=5000; return true;
      case 105: lo=0; hi=24; return true;
      case 59: lo=0; hi=11; return true;
      case 60: lo=0; hi=2000; return true;
      case 61: lo=0; hi=100; return true;
      case 62: lo=0; hi=11; return true;
      case 63: lo=0; hi=2000; return true;
      case 64: lo=0; hi=100; return true;
      case 65: lo=0; hi=5000; return true;
      case 66: lo=0; hi=5000; return true;
      case 67: lo=0; hi=5000; return true;
      case 68: lo=0; hi=100; return true;
      case 69: lo=0; hi=5000; return true;
      case 70: lo=0; hi=100; return true;
      case 71: lo=0; hi=200; return true;
      case 72: lo=0; hi=5000; return true;
      case 73: lo=0; hi=5000; return true;
      case 74: lo=0; hi=5000; return true;
      case 75: lo=0; hi=5000; return true;
      case 76: lo=0; hi=100; return true;
      case 77: lo=0; hi=600; return true;
      case 78: lo=0; hi=5000; return true;
      case 79: lo=70; hi=500; return true;
      case 80: lo=0; hi=100; return true;
      case 81: lo=0; hi=100; return true;
      case 82: lo=0; hi=100; return true;
      case 83: lo=0; hi=100; return true;
      case 84: lo=0; hi=100; return true;
      case 85: lo=0; hi=100; return true;
      case 86: lo=0; hi=100; return true;
      case 87: lo=0; hi=2; return true;
      case 88: lo=0; hi=5000; return true;
      case 89: lo=70; hi=500; return true;
      case 90: lo=0; hi=100; return true;
      case 91: lo=0; hi=100; return true;
      case 92: lo=0; hi=100; return true;
      case 93: lo=0; hi=11; return true;
      case 94: lo=0; hi=2000; return true;
      case 95: lo=0; hi=100; return true;
      case 96: lo=0; hi=500; return true;
      case 97: lo=0; hi=200; return true;
      case 3: lo=0; hi=100; return true;
      case 120: lo=0; hi=5; return true;
      case 198: lo=0; hi=4; return true;
      case 199: lo=0; hi=1500; return true;
      case 121: lo=0; hi=100; return true;
      case 122: lo=0; hi=11; return true;
      case 123: lo=50; hi=200; return true;
      case 124: lo=0; hi=100; return true;
      case 125: lo=0; hi=11; return true;
      case 126: lo=50; hi=200; return true;
      case 127: lo=0; hi=100; return true;
      case 128: lo=0; hi=11; return true;
      case 129: lo=50; hi=200; return true;
      case 130: lo=0; hi=100; return true;
      case 131: lo=0; hi=100; return true;
      case 132: lo=0; hi=100; return true;
      case 133: lo=0; hi=100; return true;
      case 134: lo=0; hi=100; return true;
      case 135: lo=0; hi=100; return true;
      case 136: lo=0; hi=100; return true;
      case 137: lo=0; hi=5000; return true;
      case 138: lo=0; hi=5000; return true;
      case 139: lo=0; hi=5000; return true;
      case 140: lo=0; hi=100; return true;
      case 141: lo=0; hi=5000; return true;
      case 142: lo=0; hi=100; return true;
      case 143: lo=0; hi=5000; return true;
      case 144: lo=0; hi=100; return true;
      case 145: lo=70; hi=500; return true;
      case 146: lo=0; hi=5000; return true;
      case 147: lo=0; hi=5000; return true;
      case 148: lo=0; hi=5000; return true;
      case 149: lo=0; hi=100; return true;
      case 150: lo=0; hi=5000; return true;
      case 151: lo=0; hi=100; return true;
      case 152: lo=0; hi=11; return true;
      case 153: lo=0; hi=2000; return true;
      case 154: lo=0; hi=100; return true;
      case 155: lo=0; hi=500; return true;
      case 156: lo=0; hi=11; return true;
      case 157: lo=0; hi=2000; return true;
      case 158: lo=0; hi=500; return true;
      case 159: lo=0; hi=100; return true;
      case 160: lo=0; hi=11; return true;
      case 161: lo=0; hi=2000; return true;
      case 162: lo=0; hi=100; return true;
      case 163: lo=0; hi=100; return true;
      case 164: lo=0; hi=5000; return true;
      case 165: lo=0; hi=5000; return true;
      case 166: lo=0; hi=5000; return true;
      case 167: lo=0; hi=100; return true;
      case 168: lo=0; hi=5000; return true;
      case 169: lo=0; hi=100; return true;
      case 170: lo=0; hi=200; return true;
      case 171: lo=0; hi=5000; return true;
      case 172: lo=0; hi=5000; return true;
      case 173: lo=0; hi=5000; return true;
      case 174: lo=0; hi=100; return true;
      case 175: lo=0; hi=100; return true;
      case 176: lo=0; hi=600; return true;
      case 177: lo=0; hi=5000; return true;
      case 178: lo=70; hi=500; return true;
      case 179: lo=0; hi=100; return true;
      case 180: lo=0; hi=100; return true;
      case 181: lo=0; hi=100; return true;
      case 182: lo=0; hi=100; return true;
      case 183: lo=0; hi=100; return true;
      case 184: lo=0; hi=100; return true;
      case 185: lo=0; hi=100; return true;
      case 186: lo=0; hi=2; return true;
      case 187: lo=30; hi=300; return true;
      case 188: lo=1; hi=16; return true;
      case 189: lo=1; hi=8; return true;
      case 190: lo=50; hi=150; return true;
      case 191: lo=20; hi=1000; return true;
      case 220: lo=0; hi=128; return true;
      case 221: lo=0; hi=128; return true;
      case 222: lo=0; hi=128; return true;
      case 223: lo=0; hi=128; return true;
      case 224: lo=0; hi=128; return true;
      case 225: lo=0; hi=128; return true;
      case 226: lo=0; hi=128; return true;
      case 227: lo=0; hi=128; return true;
      case 228: lo=0; hi=128; return true;
      case 229: lo=0; hi=128; return true;
      case 230: lo=0; hi=128; return true;
      case 231: lo=0; hi=128; return true;
      case 232: lo=0; hi=128; return true;
      case 233: lo=0; hi=128; return true;
      case 234: lo=0; hi=128; return true;
      case 235: lo=0; hi=128; return true;
      case 192: lo=0; hi=5000; return true;
      case 193: lo=70; hi=500; return true;
      case 194: lo=0; hi=100; return true;
      case 195: lo=0; hi=100; return true;
      case 196: lo=0; hi=100; return true;
      case 197: lo=0; hi=200; return true;
  }
  return false;
}