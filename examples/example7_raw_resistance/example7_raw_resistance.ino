/* example7_raw_resistance.ino

 This example retreieves the raw resistance of the plates. This would be used
 in the case that you want to process these values yourself.

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

int ensStatus; 
int resValue;

void setup()
{
	Wire.begin();

	Serial.begin(115200);

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check that you have the correct I2C Address.");
		while(1);
	}

  Serial.println("Example 7 Raw Resistance Values.");

	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	// Device needs to be set to idle to apply any settings.
	// myENS.setOperatingMode(SFE_ENS160_IDLE);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

	// There are four values here: 
	// 0 - Operating ok: Standard Operation
	// 1 - Warm-up: occurs for 3 minutes after power-on.
	// 2 - Initial Start-up: Occurs for the first hour of operation.
  //												and only once in sensor's lifetime.
	// 3 - No Valid Output
	ensStatus = myENS.getFlags();
	Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
	Serial.println(ensStatus);
	
}

void loop()
{
	if( myENS.checkDataStatus() )
	{
        resValue = myENS.getRawResistance();
		Serial.print("Resistance Value: ");
        Serial.println(resValue);
	}

	delay(200);
}
