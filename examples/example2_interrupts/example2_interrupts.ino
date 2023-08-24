/* example2_interrupts.ino

 This example shows how to configure interrupts for the SparkFun 
 Indoor Air Quality Sensor - ENS160.

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

int ensInt = 1; 
int ensStatus; 

void setup()
{
	pinMode(ensInt, INPUT_PULLUP);

	Wire.begin();

	Serial.begin(115200);

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

  Serial.println("Example 2 Interrupts.");

	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	// Device needs to be set to idle to apply the following settings.
	myENS.setOperatingMode(SFE_ENS160_IDLE);

	//myENS.configureInterrupt(0x61); //If you want to do the following stuff but all at once.

	myENS.enableInterrupt();			// Enable physical interrupt.
	myENS.setDataInterrupt();			// Enable the data ready bit on the physical interrupt pin.
	//myENS.setInterruptPolarity(); // Changes interrupt from active low to active high.
	//myENS.setInterruptDrive();    //Changes interrupt from active low to push-pull.

	// Set to standard operation
	// Others args include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

	delay(100);

	
	// There are four values here: 
	// 0 - Operating ok: Standard Opepration
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
	if( digitalRead(ensInt) == LOW )
	{
		Serial.print("Air Quality Index (1-5): ");
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
