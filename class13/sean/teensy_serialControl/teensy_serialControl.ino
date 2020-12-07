/*
  Potentiometers (7):
  A0, A1, A2, A3, A6, A7, A8
  Pins 13, 15, 16, 17, 20, 21

  Pushbuttons: (3)
  D0, D1, D2

  Rotary Encoders (2)
  D3, D4, D5, D6

  Rotary Encoder Buttons (2)
  D7, D8

  i2c Chain (sparkfun joystick, VCNL light-proximity, twist encoder, capacitive touch breakout)
  SDA1, SCL1
  Pins 18, 19


*/
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h"
#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <Encoder.h>

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

Encoder knobLeft(3, 4);
Encoder knobRight(5, 6);

JOYSTICK joystick;
int joystickVal = 0;

TWIST twist;
#define NUMBYTES 3
int incomingByte[NUMBYTES];

VCNL4040 proximitySensor;


// analog values array size, must be constant
const int num_of_analog_pins = 7; // total numer of analog pins
int analog_values[num_of_analog_pins];
int analog_pins[] = {
  0, 1, 2, 3, 6, 7, 8// list each analog pin to use
};


// digital_values array size, for push button of encoder
const int num_of_digital_pins = 5; // total numer of digital pins
int digital_values[num_of_digital_pins];
int digital_pins [] = {
  0, 1, 2, 7, 8 // list each digital pin to use
};


// SETUP ===================================


void setup() {
  pinMode(0, INPUT_PULLUP); //  pushbutton
  pinMode(1, INPUT_PULLUP); //  pushbutton
  pinMode(2, INPUT_PULLUP); //  pushbutton
  pinMode(7, INPUT_PULLUP); // encoder pushbutton
  pinMode(8, INPUT_PULLUP); // encoder pushbutton


  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  if (joystick.begin() == false)
  {
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  if (twist.begin() == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Optional: After Twist begin(), increase I2C speed to max, 400kHz
  twist.setColor(100, 10, 50); //Set the Red, Green, and Blue LED brightnesses

  Wire.begin(); //Join i2c bus
  proximitySensor.begin(); //Initialize the sensor

  //Turn on everything
  proximitySensor.powerOnProximity();
  proximitySensor.powerOnAmbient();
  proximitySensor.enableWhiteChannel();


  delay(1000);


}

long positionLeft  = -999;
long positionRight = -999;

// LOOP ===================================

void loop() {

  // CAPACITIVE MPR121
  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print("Cap: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(1);
      Serial.print(" ");
      Serial.println(" ");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print("Cap: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(0);
      Serial.print(" ");
      Serial.println(" ");
    }
  }

  // reset our state
  lasttouched = currtouched;

  // ROTARY ENCODERS

  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {

    Serial.print("Left: ");
    Serial.print(newLeft);
    Serial.print(" ");
    Serial.println(" ");

    Serial.print("Right: ");
    Serial.print(newRight);
    Serial.print(" ");
    Serial.println(" ");

    positionLeft = newLeft;
    positionRight = newRight;
  }


  // JOYSTICK
  Serial.print("jx: ");
  Serial.print(joystick.getHorizontal());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("jy: ");
  Serial.print(joystick.getVertical());
  Serial.print(" ");
  Serial.println(" ");

  joystickVal = (joystick.getButton());
  if (joystickVal == 1) {
    joystickVal = 0;
  } else {
    joystickVal = 1;
  }

  Serial.print("button: ");
  Serial.print(joystickVal);
  Serial.print(" ");
  Serial.println(" ");

  // TWIST ENCODER

  // look for 3 bytes of data and write them to the 3 output pins
  if (Serial.available() > 1) {

    for ( int i = 0; i < 3; i++) {
      incomingByte[i] = Serial.read();
    }
    twist.setColor(incomingByte[0], incomingByte[1], incomingByte[2]);
  }


  Serial.print("count: ");
  Serial.print(twist.getCount());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("difference: ");
  Serial.print(twist.getDiff());
  Serial.print(" ");
  Serial.println(" ");

  if (twist.isPressed()) {
    Serial.print("pressed: ");
    Serial.print(" ");
    Serial.println();
  }

  // VCNL4040 LIGHT / PROXIMITY

  unsigned int proxValue = proximitySensor.getProximity();
  Serial.print("proximity: ");
  Serial.print(proxValue);
  Serial.print(" ");
  Serial.println(" ");

  unsigned int ambientValue = proximitySensor.getAmbient();
  Serial.print("ambientlight: ");
  Serial.print(ambientValue);
  Serial.print(" ");
  Serial.println(" ");

  unsigned int whiteValue = proximitySensor.getWhite();
  Serial.print("whitelight: ");
  Serial.print(whiteValue);
  Serial.print(" ");
  Serial.println(" ");


  // ANALOG AND DIGITAL PINS

  for (int i = 0; i < num_of_analog_pins; i++) analog_values[i] = analogRead(analog_pins[i]);
  for (int i = 0; i < num_of_digital_pins; i++) digital_values[i] = !digitalRead(digital_pins[i]);

  Serial.print("analog_values: ");
  for (int i = 0; i < (num_of_analog_pins); i++) {
    Serial.print(analog_values[i]);
    Serial.print(" ");
  }
  Serial.println(" ");


  Serial.print("digital_values: "); // encoder pushbutton
  for (int i = 0; i < (num_of_digital_pins); i++) {
    Serial.print(digital_values[i]);
    Serial.print(" ");
  }
  Serial.println(" ");

  delay(20);
}


/*

  Code sources:

  Code adapted from Alexandros Drymonits, Yann Seznec, Sparkfun, Adafruit and PJRC

  Encoder Library - TwoKnobs Example
  http://www.pjrc.com/teensy/td_libs_Encoder.html
  Demo with Adafruit rotary encoder https://www.adafruit.com/product/377

  MPR121 code adapted from...

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

*/
