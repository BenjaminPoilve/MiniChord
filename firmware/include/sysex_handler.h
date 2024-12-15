void apply_audio_parameter(int adress, int value) {
    switch(adress){
      case 20:
        bank_led_hue=value; set_led_color(bank_led_hue, 1.0, 1.0);
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
        chord_pot.set_alternate(value);
        break;
      case 11:
        chord_pot.set_alternate_range(value);
        break;
      case 12:
        harp_pot.set_alternate(value);
        break;
      case 13:
        harp_pot.set_alternate_range(value);
        break;
      case 14:
        mod_pot.set_main(value);
        break;
      case 15:
        mod_pot.set_main_range(value);
        break;
      case 16:
        mod_pot.set_alternate(value);
        break;
      case 17:
        mod_pot.set_alternate_range(value);
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
        string_gain.amplitude(value/100.0,100);  harp_pot.update_parameter(false);  usbMIDI.sendControlChange(midi_volume_control,127*value/100.0, 0,harp_port);
        break;
      case 40:
        for (int i=0;i<12;i++){
          harp_shuffling_selection=value; current_harp_notes[i]=calculate_note_harp(i,slash_chord,sharp_active);
        }
        break;
      case 41:
        for (int i=0;i<12;i++){
          string_waveform_array[i]->amplitude(value/100.0);
        }
        break;
      case 42:
        for (int i=0;i<12;i++){
          string_waveform_array[i]->begin(value);
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
      case 59:
        string_tremolo_lfo.begin(value);
        break;
      case 60:
        string_tremolo_lfo.frequency(value/100.0);
        break;
      case 61:
        string_tremolo_lfo.amplitude(0.01+value/100.0);string_tremolo_lfo.offset(1-value/100.0);
        break;
      case 62:
        string_vibrato_lfo.begin(value);
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
        reverb_mixer.gain(0,value/100.0);stereo_r_mixer.gain(0,(1-reverb_dry_proportion*value/100.0)*pan);stereo_l_mixer.gain(0,1-reverb_dry_proportion*value/100.0);
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
        string_filter_lfo.begin(value);
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
        chords_gain.amplitude(value/100.0,100);  chord_pot.update_parameter(false); usbMIDI.sendControlChange(midi_volume_control,value/100.0*127, 0,chord_port);
        break;
      case 120:
        for (int i=0;i<7;i++){
          chord_shuffling_selection=value; current_chord_notes[i]=calculate_note_chord(i,slash_chord,sharp_active);
        }
        break;
      case 121:
        for (int i=0;i<4;i++){
          chord_osc_1_array[i]->amplitude(value/100.0);
        }
        break;
      case 122:
        for (int i=0;i<4;i++){
          chord_osc_1_array[i]->begin(value);
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
          chord_osc_2_array[i]->begin(value);
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
          chord_osc_3_array[i]->begin(value);
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
        for (int i=0;i<4;i++){
          chord_filter_base_freq=value;
        }
        break;
      case 144:
        for (int i=0;i<4;i++){
          chord_filter_keytrack=value/100.0;
        }
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
        chords_filter_LFO.begin(value);
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
          chords_tremolo_lfo.begin(value);
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
          chords_vibrato_lfo.begin(value);
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
          chord_osc_1_array[i]->frequencyModulation(value/100.0);chord_osc_2_array[i]->frequencyModulation(value/100.0);chord_osc_3_array[i]->frequencyModulation(value/100.0);
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
        reverb_mixer.gain(1,value/100.0);stereo_r_mixer.gain(1,1-reverb_dry_proportion*value/100.0);stereo_l_mixer.gain(1,(1-reverb_dry_proportion*value/100.0)*pan);
        break;
      case 185:
        chord_waveshaper_mix.gain(0,1-value/100.0);chord_waveshaper_mix.gain(1,value/100.0);
        break;
      case 186:
        ws_sin_param=value;calculate_ws_array(); chord_waveshape.shape(wave_shape,257);
        break;
      case 187:
        rythm_bpm=value;recalculate_timer();
        break;
      case 188:
        rythm_loop_length=value;
        break;
      case 189:
        rythm_limit_change_to_every=value;
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