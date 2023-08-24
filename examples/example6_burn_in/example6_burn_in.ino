/* example6_burn_in.ino

 This example demonstrates the warm up phase of the ENS160. After the "burn-in" phase 
 the readings from the ENS160 will be more accurate. Before any data is given, the  
 the sensor waits for the status flag to return "Initial Start Up" or "Normal Operation".
 This time take approximately three minutes.

 Written by: 
	Elias Santistevan @ SparkFun Electronics August, 2022

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

void setup()
{
	Wire.begin();

	Serial.begin(115200);


	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

  Serial.println("Example 6 Burn In Time for the ENS160.");

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
  // and only once in sensor's lifetime.
	// 3 - No Valid Output
  Serial.println("Waiting for the device to warm up, this will take ~3 minutes.");
	Serial.println("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
	while(true)
  {
    ensStatus = myENS.getFlags();

    if((ensStatus == 2) | (ensStatus == 0))
    {
      Serial.println("ENS160 is warmed up!");
      break;
    }
    
    if(ensStatus == 3)
    {
      Serial.println("Invalid Ouput...Freezing.");
      while(1);
    }

    Serial.print(".");
    delay(250);
  }

  Serial.println();
    
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

		Serial.println();
	}

	delay(200);
}
