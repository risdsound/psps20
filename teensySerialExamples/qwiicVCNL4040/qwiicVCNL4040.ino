/*
  Based on code by  Nathan Seidle, SparkFun Electronics, April 17th, 2018
*/

#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;

void setup()
{
  Serial.begin(9600);

  Wire.begin(); //Join i2c bus

  proximitySensor.begin(); //Initialize the sensor

  //Turn on everything
  proximitySensor.powerOnProximity();
  proximitySensor.powerOnAmbient();
  proximitySensor.enableWhiteChannel();
}

void loop()
{
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

  delay(10);
}
