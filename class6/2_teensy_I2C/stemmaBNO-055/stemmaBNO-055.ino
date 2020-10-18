#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver reads raw data from the BNO055
Based on sample code from Adafruit, 2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

}

void loop(void)
{

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

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
