#include "button_matrix.h"

button_matrix::  button_matrix(uint8_t d_in, uint8_t storage_clock, uint8_t shift_clock, uint8_t read_pin_1, uint8_t read_pin_2,uint8_t read_pin_3){
    this->d_in = d_in;
    this->storage_clock = storage_clock;
    this->shift_clock = shift_clock;
    this->read_pin_1=read_pin_1;
    this->read_pin_2=read_pin_2;
    this->read_pin_3=read_pin_3;
}  

void button_matrix::setup(){
    pinMode(d_in, OUTPUT);
    pinMode(storage_clock, OUTPUT);
    pinMode(shift_clock, OUTPUT);
    pinMode(read_pin_1,INPUT);
    pinMode(read_pin_2,INPUT);
    pinMode(read_pin_3,INPUT);

}

void button_matrix::write_bit(bool data){
    digitalWrite(shift_clock, LOW);
    if(data) digitalWrite(d_in, HIGH);
    else digitalWrite(d_in, LOW);
    digitalWrite(shift_clock, HIGH);
}

void button_matrix::write_byte(byte data){
    byte mask = 0x1;
    byte bits = data;
    digitalWrite(storage_clock, LOW);
    for(int i = registersBits-1; i >= 0; i--){
        write_bit(bits & mask);
        bits = bits >> 1;
    }
    digitalWrite(storage_clock, HIGH);
}

void button_matrix::update(debouncer (&data_array)[22]){
    byte index=7;
    byte mask = 0x1;
    while (index>0){
        write_byte(~(mask << index));
        //wait for the signal
        delayMicroseconds(5);
        bool reading=!digitalRead(read_pin_1);
        uint8_t write_index=index*3-2;
        data_array[write_index].set(reading);
        reading=!digitalRead(read_pin_2);
        write_index=index*3-1;
        data_array[write_index].set(reading);
        reading=!digitalRead(read_pin_3);
        write_index=index*3;
        data_array[write_index].set(reading);
        index=index-1;
  }
  //now the sharp
  write_byte(~(mask));
  delayMicroseconds(5);
  bool reading=!digitalRead(read_pin_1);
  data_array[0].set(reading);
}