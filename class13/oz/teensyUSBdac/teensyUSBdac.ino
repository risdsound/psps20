/*
 for Teensy 3.2, 
 
 connect DAC pin 14 to signal of audio jack (tip of TS jack)
 connect ground pin from Teensy to ground of audio jack (sleeve of TS jack)
 run to an amplifier 
 amplifer to speaker

 when programming in Teensyduino make sure to set Tool > USB Type > Audio

*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputUSB            usb1;           
AudioOutputAnalog        dac1;          
AudioConnection          patchCord1(usb1, 0, dac1, 0);

void setup() {                
  AudioMemory(12);
}

void loop() {
  
}
