/*

  This example demonstrates how to send serial data to Pd for
  - 4 I2C sensors: Qwiic Joystick, Qwiic Twist, Qwiic VCNL 4040, and Adafruit Stemma BNO055
  - 4 analog inputs (potentiometers), setting up individually and using ResponsiveAnalogRead library
  - 4 digital inputs (push buttons), setting up individually

*/

// include libraries install under Tools > Manage Libraries in Arduino

#include <Wire.h>
#include <ResponsiveAnalogRead.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h"
#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


JOYSTICK joystick;
int joystickVal = 0;

TWIST twist;
#define NUMBYTES 3
int incomingByte[NUMBYTES];

VCNL4040 proximitySensor;

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

// analog pins, setting up individually in this example

const int APIN0 = A0;
const int APIN1 = A1;
const int APIN2 = A2;
const int APIN3 = A3;
ResponsiveAnalogRead analog0(APIN0, true);
ResponsiveAnalogRead analog1(APIN1, true);
ResponsiveAnalogRead analog2(APIN2, true);
ResponsiveAnalogRead analog3(APIN3, true);


// digital pins, setting up individually in this example

const int digital0 = 0;
const int digital1 = 1;
const int digital2 = 2;
const int digital3 = 3;
int digital_state_0 = 0;
int digital_state_1 = 0;
int digital_state_2 = 0;
int digital_state_3 = 0;

//

elapsedMillis msec = 0; // don't change


void setup() {

  // analogReadResolution(10); // Teensy can change bit resolution
  // analogReadAveraging(32); // Teensy can do averaging of samples in hardware, set to 4, 8, 16, 32

  pinMode(0, INPUT_PULLUP);  // set pin modes individually
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);


  Serial.begin(9600);

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

  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
}


void loop() {

  // establish a speed limit
  if (msec >= 20) {
    msec = 0;

    // analog pins
    analog0.update();
    analog1.update();
    analog2.update();
    analog3.update();

    Serial.print("analog_values: ");
    Serial.print(analog0.getValue());
    Serial.print(" ");
    Serial.print(analog1.getValue());
    Serial.print(" ");
    Serial.print(analog2.getValue());
    Serial.print(" ");
    Serial.print(analog3.getValue());
    Serial.print(" ");
    Serial.println(" ");


    // digital pins

    digital_state_0 = digitalRead(digital0);
    digital_state_1 = digitalRead(digital1);
    digital_state_2 = digitalRead(digital2);
    digital_state_3 = digitalRead(digital3);

    Serial.print("digital_values: ");

    if (digital_state_0 == LOW) {
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
    Serial.print(" ");

    if (digital_state_1 == LOW) {
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
    Serial.print(" ");

    if (digital_state_2 == LOW) {
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
    Serial.print(" ");

    if (digital_state_3 == LOW) {
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
    Serial.print(" ");
    Serial.println(" ");

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

    // twist encoder

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

    // proximity sensor

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

    // 9 DoF motion sensor

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

  }
}
