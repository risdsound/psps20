/*

*/

#include <Wire.h>
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


void setup() {
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

  // proximity

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

  delay(50);
}
