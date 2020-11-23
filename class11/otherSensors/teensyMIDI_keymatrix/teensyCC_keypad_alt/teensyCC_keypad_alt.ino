#include "Arduino.h"
#include "Keypad.h"

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


const int channel = 1; // don't change unless you know what you are doing

elapsedMillis msec = 0; // don't change

void ccled (byte channel, byte number, byte value) {

  // D0

  if (number == 70 && value == 127) {
    digitalWrite(14, HIGH);
  } else if (number == 70 && value == 0) {
    digitalWrite(14, LOW);
  }

  //D1

  if (number == 71 && value == 127) {
    digitalWrite(15, HIGH);
  } else if (number == 71 && value == 0) {
    digitalWrite(15, LOW);
  }

  // D2

  if (number == 72 && value == 127) {
    digitalWrite(16, HIGH);
  } else if (number == 72 && value == 0) {
    digitalWrite(16, LOW);
  }

  // D3

  if (number == 73 && value == 127) {
    digitalWrite(17, HIGH);
  } else if (number == 73 && value == 0) {
    digitalWrite(17, LOW);
  }

  // D4

  if (number == 74 && value == 127) {
    digitalWrite(18, HIGH);
  } else if (number == 74 && value == 0) {
    digitalWrite(18, LOW);
  }

  // D5

  if (number == 75 && value == 127) {
    digitalWrite(19, HIGH);
  } else if (number == 75 && value == 0) {
    digitalWrite(19, LOW);
  }
}


void setup() {
  Serial.begin(9600);

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

}

void loop() {
  if (msec >= 20) {
    msec = 0;
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '1') {
      usbMIDI.sendControlChange(51, 127, channel);
    }
    if (key == '2') {
      usbMIDI.sendControlChange(52, 127, channel);
    }
    if (key == '3') {
      usbMIDI.sendControlChange(53, 127, channel);
    }
    if (key == '4') {
      usbMIDI.sendControlChange(54, 127, channel);
    }
    if (key == '5') {
      usbMIDI.sendControlChange(55, 127, channel);
    }
    if (key == '6') {
      usbMIDI.sendControlChange(56, 127, channel);
    }
    if (key == '7') {
      usbMIDI.sendControlChange(57, 127, channel);
    }
    if (key == '8') {
      usbMIDI.sendControlChange(58, 127, channel);
    }
    if (key == '9') {
      usbMIDI.sendControlChange(59, 127, channel);
    }
    if (key == '0') {
      usbMIDI.sendControlChange(60, 127, channel);
    }
    if (key == '*') {
      usbMIDI.sendControlChange(61, 127, channel);
    }
    if (key == '#') {
      usbMIDI.sendControlChange(62, 127, channel);
    }

  }
  usbMIDI.setHandleControlChange(ccled) ;
  usbMIDI.read();
}

