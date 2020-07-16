/*
Based on code by: Nathan Seidle, SparkFun Electronics, December 3rd, 2018
*/

#include "SparkFun_Qwiic_Twist_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_Twist
TWIST twist; //Create instance of this object

#define NUMBYTES 3 // number of bytes  sent by Pd in a "packet"
int incomingByte[NUMBYTES];


void setup() {
  Serial.begin(9600);

  if (twist.begin() == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Optional: After Twist begin(), increase I2C speed to max, 400kHz

  //Set the knob color to pink
  twist.setColor(100, 10, 50); //Set the Red, Green, and Blue LED brightnesses

}

void loop() {

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


  delay(10);
}
