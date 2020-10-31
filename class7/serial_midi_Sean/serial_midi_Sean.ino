/*
  This code demonstrates using serial data for I2C sensors
  in combination with MIDI data for analog and digital input pins

  You must program with USB Type: Serial + MIDI
*/

// required libraries

#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h"

// I2C sensors - joystick only

JOYSTICK joystick;
int joystickVal = 0;


// this section sets the analog pins, change where indicated

int const numPins = 4; // change the numer to how many analog pins you will use
int currentVal[numPins];
int newVal[numPins];
int analogPins[] = {
  0, 1, 2, 3// list the numbers of each pin you will use seperated by a comma
};


// set digital pins

int const numDigPinsCC = 5; // change the number to how many digital pins you will use
int currentDigcc[numDigPinsCC];
int digitalpincc[] = {
  0, 1, 2, 3, 4 // list the numbers of each pin you will use seperated by a comma
};

int digInputcc[numDigPinsCC];

// the MIDI channel number to send messages
const int channel = 1; // don't change unless you know what you are doing

elapsedMillis msec = 0; // don't change


void setup() {

  // This tells the digital pins how to function

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);


  analogReadAveraging(32); // Teensy can do averaging of samples in hardware, set to 4, 8, 16, 32


  // set up serial for I2C sensors

  Serial.begin(9600);

  if (joystick.begin() == false)
  {
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  delay(1000);
}

void loop() {

  if (msec >= 20) {
    msec = 0;

    // digital pins input sending CC messages beginning at ctlin 51 for Pd

    for (int i = 0; i < numDigPinsCC; i++) {
      if (digitalRead(digitalpincc[i]) == 1 && currentDigcc[i] == 0) {
        usbMIDI.sendControlChange(i + 51, 0, channel);
        currentDigcc[i] = 1;
      }
      if (digitalRead(digitalpincc[i]) == 0  && currentDigcc[i] == 1) {
        usbMIDI.sendControlChange(i + 51, 127, channel);
        currentDigcc[i] = 0;
      }
    }

    // analog pins input sending CC messages beginning at ctlin 1 for Pd

    for (int i = 0; i < numPins; i++) {

      newVal[i] = analogRead(analogPins[i]);

      if (abs(newVal[i] - currentVal[i]) > 3) {
        usbMIDI.sendControlChange(i + 1, newVal[i] >> 3, channel);
        currentVal[i] = newVal[i];
      }
    }


    // MIDI Controllers can discard incoming MIDI messages.
    // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash

    while (usbMIDI.read()) {
      // ignore incoming messages

    }

    // joystick
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

  }
}
