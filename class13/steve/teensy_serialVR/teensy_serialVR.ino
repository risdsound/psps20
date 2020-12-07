/*
  BNO055 X
  Twist encoder X
  1 push button digital in X
  1 photocell analog in X
  4 touch pins X
  LED digital output
*/

#include <Wire.h>
#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

TWIST twist;
#define NUMBYTES 3
int incomingByte[NUMBYTES];

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

// analog values array size, must be constant
const int num_of_analog_pins = 1; // total numer of analog pins
int analog_values[num_of_analog_pins];
int analog_pins[] = {
  0, // list each analog pin to use
};


// digital_values array size, for push button of encoder
const int num_of_digital_pins = 1; // total numer of digital pins
int digital_values[num_of_digital_pins];
int digital_pins [] = {
  2, // list each digital pin to use
};


// touch values array size, must be constant
const int num_of_touch_pins = 11; // total numer of analog pins
int touch_values[num_of_touch_pins];
int touch_pins[] = {
  0, 1, 3, 4, // list each touch pin to use
};


// LED output

#define NUMBYTES 2 // number of bytes sent by Pd in a "packet"
int incomingByte[NUMBYTES];

void setup() {


  pinMode(2, INPUT_PULLUP);  // 

  Serial.begin(9600);

  // TWIST

  if (twist.begin() == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Optional: After Twist begin(), increase I2C speed to max, 400kHz
  twist.setColor(100, 10, 50); //Set the Red, Green, and Blue LED brightnesses

  // MOTION

  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
}

void loop() {

  // twist

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


  // 9 DoF

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  /* Display the floating point data */
  Serial.print("x: ");
  Serial.print(euler.x());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("y: ");
  Serial.print(euler.y());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("z: ");
  Serial.print(euler.z());
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

  // TOUCH PINS

  for (int i = 0; i < num_of_touch_pins; i++) touch_values[i] = touchRead(touch_pins[i]);
  Serial.print("touch_values: ");
  for (int i = 0; i < (num_of_touch_pins); i++) {
    Serial.print(touch_values[i]);
    Serial.print(" ");
  }
  Serial.println(" ");


  // digital output, assign pin and value from Pd

  // look for 2 bytes of data and write them
  if (Serial.available() > 1) {
    for ( int i = 0; i < 2; i++) {
      incomingByte[i] = Serial.read();
    }
    digitalWrite(incomingByte[0], incomingByte[1]);
  }


  delay(50);
}
