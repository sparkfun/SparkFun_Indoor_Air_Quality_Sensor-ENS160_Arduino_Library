/* example1_basic.ino

 This example shows basic data retrieval from the SparkFun Indoor Air Quality Sensor - ENS160.

 Written by: 
	Elias Santistevan @ SparkFun Electronics October, 2022

 Product: 
  https://www.sparkfun.com/products/20844
 
 Repository:
	https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library

 SparkFun code, firmware, and software is released under the MIT
 License(http://opensource.org/licenses/MIT).

*/
#include <Wire.h>
#include "SparkFun_ENS160.h"

SparkFun_ENS160 myENS; 


void setup()
{
	Wire.begin();

	Serial.begin(115200);

	if( !myENS.begin() )
	{
		Serial.println("Did not begin.");
		while(1);
	}

	Serial.println("Ready.");
	
}

void loop()
{
	if( myENS.checkDataStatus() )
	{
		Serial.print("Air Quality Index (1-5) : ");
		Serial.println(myENS.getAQI());

		Serial.print("Total Volatile Organic Compounds: ");
		Serial.print(myENS.getTVOC());
		Serial.println("ppb");

		Serial.print("CO2 concentration: ");
		Serial.print(myENS.getECO2());
		Serial.println("ppm");
	}

	delay(200);
}
