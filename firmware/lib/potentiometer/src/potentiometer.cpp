#include "potentiometer.h"
#include <Audio.h>



potentiometer::potentiometer(int pot_pin){
    this->pot_pin = pot_pin;
}  

void potentiometer::setup(int main_adress,float main_range,int alternate_adress,float alternate_range, int16_t *current_sysex_parameters_pointer,int alternate_initial_value,const std::function<void(int, int)> & apply_audio_parameter, int alternate_storage_adress){
    this->main_adress = main_adress;
    this->main_range = main_range;
    this->alternate_adress = alternate_adress;
    this->alternate_range=alternate_range;
    this->current_sysex_parameters_pointer=current_sysex_parameters_pointer;
    this->alternate_value=alternate_initial_value;
    this->apply_audio_parameter=apply_audio_parameter;
    this->alternate_storage_adress=alternate_storage_adress;
}  
void potentiometer::set_main(int adress){
    this->main_adress = adress;
}
void potentiometer::set_main_range(float range){
    this->main_range = range;
}
void potentiometer::set_alternate(int adress){
    this->alternate_adress = adress;
}
void potentiometer::set_alternate_range(float range){
    this->alternate_range = range;
}

void potentiometer::set_alternate_default(int alternate_initial_value){
    this->alternate_value=alternate_initial_value;
}


// Looks up the bounds declared for a selector parameter. Returns false for
// everything else -- floats, and integers with a wide continuous range such as
// times and frequencies -- which keep the existing proportional behaviour.
// generator/generate.py decides which parameters are selectors.
bool potentiometer::declared_bounds(int adress, int16_t &min_out, int16_t &max_out){
    for(unsigned i=0;i<sizeof(parameter_lookup)/sizeof(parameter_lookup[0]);i++){
        if(parameter_lookup[i].sysex_adress==adress){
            if(!parameter_lookup[i].is_selector) return false;
            min_out=parameter_lookup[i].min_value;
            max_out=parameter_lookup[i].max_value;
            return true;
        }
    }
    return false;
}

// Holds the current value until the reading has moved a fair way past the step
// boundary, so a control resting on a boundary does not flip on noise alone.
int16_t potentiometer::quantise(int16_t proposed, int16_t lo, int16_t hi, int adress){
    if(adress!=last_discrete_adress){
        last_discrete_adress=adress;
        last_discrete_output=proposed;
        last_discrete_reading=potentiometer_smoothed_value;
        return proposed;
    }
    if(proposed==last_discrete_output) return proposed;
    int16_t steps=hi-lo;
    if(steps<=0){
        last_discrete_output=proposed;
        last_discrete_reading=potentiometer_smoothed_value;
        return proposed;
    }
    float step_width=(float)(1024-2*dead_zone)/(float)(steps+1);
    if(abs(potentiometer_smoothed_value-last_discrete_reading) < step_width*0.6f){
        return last_discrete_output;
    }
    last_discrete_output=proposed;
    last_discrete_reading=potentiometer_smoothed_value;
    return proposed;
}

void potentiometer::force_update(){
    potentiometer_smoothed_value=0;
    potentiometer_old_value=1024;// bit of a hack to trigger an update of the main parameter
    update_parameter(false); //apply to the main the current value of the potentiometer.
    int16_t d_min, d_max;
    bool discrete=declared_bounds(alternate_adress, d_min, d_max);
    uint16_t min_value = discrete ? d_min : max(0,current_sysex_parameters_pointer[alternate_adress]*(1.0-alternate_range/100.0));
    uint16_t max_value = discrete ? d_max : current_sysex_parameters_pointer[alternate_adress]*(1.0+alternate_range/100.0);
    uint16_t output_value=constrain(map(alternate_value, dead_zone, 1024-dead_zone,min_value ,max_value ),min_value, max_value);
    if(discrete){ last_discrete_adress=alternate_adress; last_discrete_output=output_value; last_discrete_reading=alternate_value; }
    apply_audio_parameter(alternate_adress, output_value); //now apply the value that was saved for the alternate. That allows to memorize the settings of the user.
}

void potentiometer::pickup_on_next_switch(){
    pickup_next=true;
}

bool potentiometer::update_parameter(bool alternate_flag){
    uint16_t current_reading = 1024-analogRead(pot_pin);
    potentiometer_smoothed_value=(10*potentiometer_smoothed_value+current_reading)/11;
    // Switching between main and alternate: after a switch that asked for pickup, the newly active
    // function keeps its value until the knob comes back to the position it was last set from.
    if(alternate_flag!=last_alternate){
        last_alternate=alternate_flag;
        int held=alternate_flag ? alternate_value : main_position;
        waiting = pickup_next && held>=0;
        if(waiting) waiting_side = potentiometer_smoothed_value>held ? 1 : -1;
        pickup_next=false;
        potentiometer_old_value=potentiometer_smoothed_value;   // the switch itself isn't a turn of the knob
    }
    if(waiting){
        int held=last_alternate ? alternate_value : main_position;
        int8_t side = potentiometer_smoothed_value>held ? 1 : -1;
        if(abs(potentiometer_smoothed_value-held)<=threshold || side!=waiting_side){
            waiting=false;                                      // reached (or crossed) it: the knob takes over from here
            potentiometer_old_value=potentiometer_smoothed_value;
        }
        return false;
    }
    //let's see if one of the register was modified in the meantime, that would justify a change 
    //it's a bit verbose, but the issue is that we want to allow the interface to modify the parameter, while taking into account the pot position 
    bool trigger_change=false;
    //here we want to make sure that primary values are always the one selected 
    if(!alternate_flag){
        if(current_sysex_parameters_pointer[main_adress]!=old_main_adress_value){
            trigger_change=true;
            old_main_adress_value=current_sysex_parameters_pointer[main_adress];
        }
    }

    if(abs(potentiometer_old_value-potentiometer_smoothed_value)>threshold || trigger_change){
        trigger_change=false;
        this->potentiometer_old_value=potentiometer_smoothed_value;
        if(!alternate_flag ){
            int16_t d_min, d_max;
            bool discrete=declared_bounds(main_adress, d_min, d_max);
            uint16_t min_value = discrete ? d_min : max(0,current_sysex_parameters_pointer[main_adress]*(1.0-main_range/100.0));
            uint16_t max_value = discrete ? d_max : current_sysex_parameters_pointer[main_adress]*(1.0+main_range/100.0);
            uint16_t output_value=constrain(map(potentiometer_smoothed_value, dead_zone,1024-dead_zone, min_value ,max_value ),min_value, max_value);
            if(discrete) output_value=quantise(output_value, min_value, max_value, main_adress);
            Serial.println(output_value);
            apply_audio_parameter(main_adress, output_value); //note: applied but not saved. So we can still read the initial value 
            main_position=potentiometer_smoothed_value;     // where the main function was last set from, for pickup
            update_parameter(false); //loop again for the smoothing
        }else{
            int16_t d_min, d_max;
            bool discrete=declared_bounds(alternate_adress, d_min, d_max);
            uint16_t min_value = discrete ? d_min : max(0,current_sysex_parameters_pointer[alternate_adress]*(1.0-alternate_range/100.0));
            uint16_t max_value = discrete ? d_max : current_sysex_parameters_pointer[alternate_adress]*(1.0+alternate_range/100.0);
            uint16_t output_value=constrain(map(potentiometer_smoothed_value, dead_zone, 1024-dead_zone,min_value ,max_value ),min_value, max_value);
            if(discrete) output_value=quantise(output_value, min_value, max_value, alternate_adress);
            apply_audio_parameter(alternate_adress, output_value);
            this->alternate_value=potentiometer_smoothed_value;
            current_sysex_parameters_pointer[alternate_storage_adress] = potentiometer_smoothed_value; // saving the position in order to be able to retrieve it later
            return true;
        }
        
    }
    return false;
}