/*
  Based on code by by Wes Furuya @ SparkFun Electronics, February 5th, 2019
*/

#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_joystick
JOYSTICK joystick; //Create instance of this object
int joystickVal = 0; 

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Joystick Example");

  if(joystick.begin() == false)
  {
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while(1);
  }
}

void loop() {
  
  Serial.print("x: ");
  Serial.print(joystick.getHorizontal());
  Serial.print(" ");
  Serial.println(" ");

  Serial.print("y: ");
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
  


  delay(50);
}
