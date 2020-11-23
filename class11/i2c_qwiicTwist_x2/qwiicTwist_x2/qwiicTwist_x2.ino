/*
  Based on code by: Nathan Seidle, SparkFun Electronics, December 3rd, 2018

  To use a second Twist encoder, you must change the address of one with the change address sample code
  
*/

#include "SparkFun_Qwiic_Twist_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_Twist
TWIST twist1; //Create instance of this object
TWIST twist2; //Create instance of this object

#define NUMBYTES 6 // number of bytes  sent by Pd in a "packet"
int incomingByte[NUMBYTES];

void setup() {
  Serial.begin(9600);


  if (twist1.begin(Wire, 0x3F) == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

    if (twist2.begin(Wire, 0x77) == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Optional: After Twist begin(), increase I2C speed to max, 400kHz

  //Set the knob color to pink
  twist1.setColor(100, 10, 50); //Set the Red, Green, and Blue LED brightnesses
  twist2.setColor(10, 100, 255); //Set the Red, Green, and Blue LED brightnesses


}

void loop() {

  // look for 3 bytes of data and write them to the 3 output pins
  if (Serial.available() > 1) {

    for ( int i = 0; i < 6; i++) {
      incomingByte[i] = Serial.read();
    }
    twist1.setColor(incomingByte[0], incomingByte[1], incomingByte[2]);
    twist2.setColor(incomingByte[3], incomingByte[4], incomingByte[5]);

  }


  Serial.print("count1: ");
  Serial.print(twist1.getCount());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("difference1: ");
  Serial.print(twist1.getDiff());
  Serial.print(" ");
  Serial.println(" ");

  if (twist1.isPressed()) {
    Serial.print("pressed1: ");
    Serial.print(" ");
    Serial.println();
  }


  Serial.print("count2: ");
  Serial.print(twist2.getCount());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("difference2: ");
  Serial.print(twist2.getDiff());
  Serial.print(" ");
  Serial.println(" ");

  if (twist2.isPressed()) {
    Serial.print("pressed2: ");
    Serial.print(" ");
    Serial.println();
  }

  delay(20);
}
