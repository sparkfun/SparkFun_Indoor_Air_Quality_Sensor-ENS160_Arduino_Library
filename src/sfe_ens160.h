// sfe_ens160.h
//
// This is a library written for SparkFun Indoor Air Quality Sensor - ENS160 (Qwiic)
//
// SparkFun sells these boards at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
// SparkFun Indoor Air Quality Sensor - ENS160 (Qwiic)    https://www.sparkfun.com/products/20844
//
// Written by Elias Santistevan @ SparkFun Electronics, October 2022
//
//Repository:
//		https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library
//
// SparkFun code, firmware, and software is released under the MIT
// License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// The following class implements the	methods to set, get, and read from the ENS160 Air Quality
// Sensor. 

#include "sfe_bus.h"


#define ENS160_ADDRESS_LOW 0x52
#define ENS160_ADDRESS_HIGH 0x53

#define ENS160_DEVICE_ID 0x0160

class QwDevENS160
{
	public: 

		QwDevENS160() : _i2cAddress{0}, _cs{0} {};
			

    ///////////////////////////////////////////////////////////////////////
    // init()
    //
    // Called to init the system. Connects to the device and sets it up for 
    // operation

    bool init();

    ///////////////////////////////////////////////////////////////////////
    // isConnected()
    //
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  retval      true if device is connected, false if not connected

    bool isConnected(); // Checks if sensor ack's the I2C request
												
    //////////////////////////////////////////////////////////////////////////////////
    // writeRegisterRegion()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  reg          register to write to
    //  data         Array to store data in
    //  length       Length of the data being written in bytes 
    //  retval       -1 = error, 0 = success

    int32_t writeRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // readRegisterRegion()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  reg          register to read from
    //  data         Array to store data in
    //  length       Length of the data to read in bytes
    //  retval       -1 = error, 0 = success

    int32_t readRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // setCommunicationBus()
    //
    // Called to set the Communication Bus object to use
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  theBus       The Bus object to use
    //  idBus        The bus ID for the target device.
    //

		void setCommunicationBus(QwIDeviceBus &theBus, uint8_t i2cAddress);
		void setCommunicationBus(QwIDeviceBus &theBus);



    //////////////////////////////////////////////////////////////////////////////////
    // Operating Mode 0x10
    // More operating Mode (status) 0x20
		// Interrupts
		// Temp Compensation	
		// Data - AQI, TVOC, ECO2, Temp (added), Relative Humidity (added)
		// General Purpose Read/Write Registers

		bool setOperatingMode(uint8_t);
		bool configureInterrupt(uint8_t);
		bool setInterruptEnable(bool);
		bool setInterruptPolarity(uint8_t);
		bool setInterruptDrive(uint8_t);
		bool setDataInterrupt(bool);
		bool setGPRInterrupt(bool);
		uint32_t getAppVer();
		bool setTempCompensation(float);
		bool setRHCompensation(uint16_t);
		bool checkDataStatus();
		bool checkGPRStatus();
		uint8_t getFlags();
		bool checkOperationMode();
		bool getError();
		uint8_t getAQI();
		uint16_t getTVOC();
		uint16_t getETOH();
		uint16_t getECO2();
		float getTempKelvin();
		float getTempCelsius();
		float getRH();

	private: 

		
		QwIDeviceBus *_sfeBus; 
		uint8_t _i2cAddress;
		uint8_t _cs;
};
