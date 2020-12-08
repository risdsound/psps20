/*********************************************************

  Code for 2 MPR121 capactive sensors and remaining touch pins on Teensy LC (33 touch points altogether)

  For wiring, see:
  https://learn.adafruit.com/adafruit-mpr121-12-key-capacitive-touch-sensor-breakout-tutorial/wiring

  To begin reading sensor data, you will need to download
  the Adafruit_MPR121 library from the Arduino library manager.

  Code adapted from...

  This is a library for the MPR121 12-channel Capacitive touch sensor

  Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/

  These sensors use I2C communicate, at least 2 pins are required
  to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution


**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifndef _BC
#define _BC(bit) (1 << (bit))
#endif


// You can have up to 4 on one i2c bus 

Adafruit_MPR121 cap = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();


// Keeps track of the last pins touched so we know when buttons are 'released'

uint16_t lasttouched = 0;
uint16_t currtouched = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;


// Teensy touch pins, touch values array size, must be constant

const int num_of_touch_pins = 9; // total numer of analog pins
int touch_values[num_of_touch_pins];
int touch_pins[] = {
  0, 1, 3, 4, 15, 16, 17, 22, 23 // list each touch pin to use
};


void setup() {
  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  // The MPR121 ADDR pin is pulled to ground and has a default I2C address of 0x5A
  // You can adjust the I2C address by connecting ADDR to other pins:
  // ADDR not connected: 0x5A
  // ADDR tied to 3V: 0x5B
  // ADDR tied to SDA: 0x5C
  // ADDR tied to SCL: 0x5D

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 1 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 1 found!");

  if (!cap2.begin(0x5B)) {  // connect ADDR pin to 3V pin to change address
    Serial.println("MPR121 2 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 2 found!");

}

void loop() {
  
  currtouched = cap.touched(); // Get the currently touched pads
  currtouched2 = cap2.touched(); // Get the currently touched pads

  // cap sensor 1

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print("Cap1: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(1);
      Serial.print(" ");
      Serial.println(" ");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print("Cap1: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(0);
      Serial.print(" ");
      Serial.println(" ");
    }
  }

  lasttouched = currtouched; // reset our state

  // cap sensor 2

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched2 & _BC(i)) && !(lasttouched2 & _BC(i)) ) {
      Serial.print("Cap2: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(1);
      Serial.print(" ");
      Serial.println(" ");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched2 & _BC(i)) && (lasttouched2 & _BC(i)) ) {
      Serial.print("Cap2: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(0);
      Serial.print(" ");
      Serial.println(" ");
    }
  }

  lasttouched2 = currtouched2;   // reset our state

  // teensy touch pins

  for (int i = 0; i < num_of_touch_pins; i++) touch_values[i] = touchRead(touch_pins[i]);
  Serial.print("touch_values: ");
  for (int i = 0; i < (num_of_touch_pins); i++) {
    Serial.print(touch_values[i]);
    Serial.print(" ");
  }
  Serial.println(" ");

  delay(20);
}
