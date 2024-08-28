#include "potentiometer.h"



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

void potentiometer::force_update(){
    potentiometer_smoothed_value=0;
    potentiometer_old_value=1024;// bit of a hack to trigger an update of the main parameter
    update_parameter(false); //apply to the main the current value of the potentiometer.
    uint16_t min_value=max(0,current_sysex_parameters_pointer[alternate_adress]*(1.0-alternate_range/100.0));
    uint16_t max_value=current_sysex_parameters_pointer[alternate_adress]*(1.0+alternate_range/100.0);
    uint16_t output_value=constrain(map(alternate_value, dead_zone, 1024-dead_zone,min_value ,max_value ),min_value, max_value);
    apply_audio_parameter(alternate_adress, output_value); //now apply the value that was saved for the alternate. That allows to memorize the settings of the user.
}

void potentiometer::update_parameter(bool alternate_flag){
    uint16_t current_reading = 1024-analogRead(pot_pin);
    potentiometer_smoothed_value=(20*potentiometer_smoothed_value+current_reading)/21;
    //let's see if one of the register was modified in the meantime, that would justify a change 
    //it's a bit verbose, but the issue is that we want to allow the interface to modify the parameter, while taking into account the pot position 
    bool trigger_change=false;
    if(!alternate_flag){
        if(current_sysex_parameters_pointer[main_adress]!=old_main_adress_value){
            trigger_change=true;
            old_main_adress_value=current_sysex_parameters_pointer[main_adress];
        }
    }else{
        if(current_sysex_parameters_pointer[alternate_adress]!=old_alternate_adress_value){
            trigger_change=true;
            old_alternate_adress_value=current_sysex_parameters_pointer[alternate_adress];
        }
    }
    if(abs(potentiometer_old_value-potentiometer_smoothed_value)>threshold || trigger_change){
        Serial.println("loop");
        trigger_change=false;
        this->potentiometer_old_value=potentiometer_smoothed_value;
        if(!alternate_flag ){
            uint16_t min_value=max(0,current_sysex_parameters_pointer[main_adress]*(1.0-main_range/100.0));
            uint16_t max_value=current_sysex_parameters_pointer[main_adress]*(1.0+main_range/100.0);
            uint16_t output_value=constrain(map(potentiometer_smoothed_value, dead_zone,1024-dead_zone, min_value ,max_value ),min_value, max_value);
            Serial.println(potentiometer_old_value);
            Serial.println("applying");
            apply_audio_parameter(main_adress, output_value); //note: applied but not saved. So we can still read the initial value 

        }else{
            uint16_t min_value=max(0,current_sysex_parameters_pointer[alternate_adress]*(1.0-alternate_range/100.0));
            uint16_t max_value=current_sysex_parameters_pointer[alternate_adress]*(1.0+alternate_range/100.0);
            uint16_t output_value=constrain(map(potentiometer_smoothed_value, dead_zone, 1024-dead_zone,min_value ,max_value ),min_value, max_value);
            apply_audio_parameter(alternate_adress, output_value);
            this->alternate_value=potentiometer_smoothed_value;
            current_sysex_parameters_pointer[alternate_storage_adress] = potentiometer_smoothed_value; // saving the position in order to be able to retrieve it later
        }
        
    }
}