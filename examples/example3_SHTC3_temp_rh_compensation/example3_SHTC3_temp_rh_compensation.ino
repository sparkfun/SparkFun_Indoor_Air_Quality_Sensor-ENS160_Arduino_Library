/* example3a_SHTC3_temp_rh_compensation.ino

 This example shows how to give the ENS160 Temperature and Relative Humidity
 Data for compensation. Note that the values that are given for compensation are not
 populated in their registers until the Air Quality Sensor is set to "Standard" operation
 and when data is ready i.e. the data ready bit is set. Also note that there will be some 
 rounding of the temperature and relative humidity values when they're given to the sensor
 and again when they're read back. 

 Please note that for compensation values the datasheet specifies that they can be written
 at any time. 

 Written by: 
	Elias Santistevan @ SparkFun Electronics October, 2022

 Products: 
  Air Quality Sensor (ENS160)             -  https://www.sparkfun.com/products/20844
  Humidity and Temperature Sensor (SHTC3) -  https://www.sparkfun.com/products/16467
 
 Repository:
	https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library

 SparkFun code, firmware, and software is released under the MIT
 License(http://opensource.org/licenses/MIT).

*/
#include <Wire.h>
#include "SparkFun_ENS160.h" // Click here to get the library: http://librarymanager/All#SparkFun_ENS160
#include "SparkFun_SHTC3.h" // Click here to get the library: http://librarymanager/All#SparkFun_SHTC3

SHTC3 mySHTC3;              // Humidity and Temp Sensor.

SparkFun_ENS160 myENS; 

bool printedCompensation = false; 
int ensStatus; 

float rh;
float tempC; 
SHTC3_Status_TypeDef result; 


void setup()
{
	Wire.begin();

	Serial.begin(115200);

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

	if( mySHTC3.begin() != SHTC3_Status_Nominal )
	{
		Serial.println("Humidity and Temperature Sensor did not begin.");
		while(1);
	}

  Serial.println("Example 3 Humidity and Temperature Sensor Compensation - SHTC3.");
	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	mySHTC3.update(); // Send command to take a measurement
	if( mySHTC3.lastStatus != SHTC3_Status_Nominal )
	{
		Serial.println("Humidity and Temperature Sensor is not ready.");
		while(1);
	}

	rh = mySHTC3.toPercent();
	tempC = mySHTC3.toDegC();
	Serial.print("Relative Humidity: ");
	Serial.println(rh);
	Serial.print("Temperature(Celsius): ");
	Serial.println(tempC);

	// Give values to Air Quality Sensor. 
	myENS.setTempCompensationCelsius(tempC); 
	myENS.setRHCompensationFloat(rh); 

	delay(500);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

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
	if( myENS.checkDataStatus() )
	{

		if( printedCompensation == false)
		{
			Serial.println("---------------------------");
			Serial.print("Compensation Temperature: ");
			Serial.println(myENS.getTempCelsius());
			Serial.println("---------------------------");
			Serial.print("Compensation Relative Humidity: ");
			Serial.println(myENS.getRH());
			Serial.println("---------------------------");
			printedCompensation = true;
			delay(500);
		}

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
