#include "debouncer.h"

debouncer::debouncer(){
}  

/* set() used to write value straight through and only gate read_transition() on
 * the debounce period. So read_value() was the raw pin state: nothing was ever
 * held steady, only the reporting of edges was delayed.
 *
 * That shows up wherever a combination of buttons is read by value rather than
 * by transition. The three chord type buttons do not release in the same
 * instant, so letting go of a major seventh reads as a plain major on the way
 * out, and anything recalculating in that window picks up the wrong chord.
 *
 * value now changes only once the reading has held for the debounce period, and
 * the flag is raised at that moment rather than at the raw edge. Transition
 * latency is unchanged - it was already the same period, just measured from the
 * other end - so anything using read_transition() behaves exactly as before.
 */
void debouncer::set(bool set_value){
    if(pending!=set_value){
        pending=set_value;
        last_update=0;
        return;
    }
    if(pending!=value && last_update>debounce_value){
        value=pending;
        flag=true;
    }
}

uint8_t debouncer::read_transition(){
    if(flag==true){
        flag=false;
        if(value==true){
            return 2;
        }else{
            return 1;  
        }
    }
    return 0;
}
bool debouncer::read_value(){
    return value;
}
