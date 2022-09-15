#include "sfe_ens160.h"
#include "sfe_ens160_regs.h"

//////////////////////////////////////////////////////////////////////////////
// init()
//
// init the system
//
// Return Value: false on error, true on success
//

bool QwDevENS160::init(void)
{
		uint16_t uniqueID; 

    //  do we have a bus yet? is the device connected?
    if (!_sfeBus->ping(_i2cAddress))
        return false;
		
		uniqueID = getUniqueID(); 
		if( uniqueID != ENS160_DEVICE_ID )
			return false; 

		return true; 

}



///////////////////////////////////////////////////////////////////////
// isConnected()
//
// 
// 
//
//  Parameter   Description
//  ---------   -----------------------------
//  retVal      true if device is connected, false if not connected

bool QwDevENS160::isConnected()
{
	uint16_t uniqueID; 

	uniqueID = getUniqueID(); 

	if( uniqueID != ENS160_DEVICE_ID )
		return false; 

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
// 
//
//
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  i2cAddress   I2C address for the 6DoF

void QwDevENS160::setCommunicationBus(QwIDeviceBus &theBus, uint8_t i2cAddress)
{
    _sfeBus = &theBus;
		_i2cAddress = i2cAddress; 

}

////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Overloaded option for setting the data bus (theBus) object to a SPI bus object.
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  

void QwDevENS160::setCommunicationBus(QwIDeviceBus &theBus)
{
    _sfeBus = &theBus;
}

//////////////////////////////////////////////////////////////////////////////
// writeRegisterRegion()
//
// Writes data to the given register
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be written to
//  data         Data to be written
//  length       Number of bytes to be written

int32_t QwDevENS160::writeRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length)
{
    return _sfeBus->writeRegisterRegion(_i2cAddress, reg, data, length);
}

//////////////////////////////////////////////////////////////////////////////
// readRegisterRegion()
//
// Reads data from the given register, auto-incrementing fore each successive read
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be read from
//  data         Pointer to where data will be saved
//  length       Number of bytes to be read

int32_t QwDevENS160::readRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length)
{
    return _sfeBus->readRegisterRegion(_i2cAddress, reg, data, length);
}


//////////////////////////////////////////////////////////////////////////////
// getUniqueID()
//
// Gets the device's unique ID 
//
uint16_t QwDevENS160::getUniqueID()
{
	int32_t retVal;
	uint8_t tempVal[2] = {0}; 
	uint16_t id; 

	retVal = readRegisterRegion(SFE_ENS160_PART_ID, tempVal, 2);
	
	id = tempVal[0];
	id |= tempVal[1] << 8;

	if( retVal != 0 )
		return 0;

	return id; 

}


//////////////////////////////////////////////////////////////////////////////
// setOperatingMode()
//
// Sets the operating mode: Deep Sleep (0x00), Idle (0x01), Standard (0x02), Reset (0xF0)
//
//  Parameter    Description
//  ---------    -----------------------------
//  val					 The desired operating mode to set. 


bool QwDevENS160::setOperatingMode(uint8_t val)
{
	int32_t retVal;

	if( val != SFE_ENS160_RESET | val > SFE_ENS160_STANDARD )
		return false;

	retVal = writeRegisterRegion(SFE_ENS160_OP_MODE, (sfe_ens160_op_mode_t)val, 1);

	if( retVal != 0 )
		return false;

	return true; 
}


//////////////////////////////////////////////////////////////////////////////
// setOperatingMode()
//
// Sets the operating mode: Deep Sleep (0x00), Idle (0x01), Standard (0x02), Reset (0xF0)
//
//  Parameter    Description
//  ---------    -----------------------------
//  val					 The desired operating mode to set. 
//

bool QwDevENS160::configureInterrupt(uint8_t val)
{
	int32_t retVal;

	// I don't think this will work casting it as the typedef struct in the case
	// that the user wants to set multiple things at once, but idk.
	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, (sfe_ens160_config_t)val, 1);

	if( retVal != 0 )
		return false;

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// setInterrupt()
//
// Enables the interrupt.
//
//  Parameter    Description
//  ---------    -----------------------------
//  enable			 Turns on or off the interrupt. 
//

bool QwDevENS160::setInterrupt(bool enable)
{
	int32_t retVal;
	sfe_ens160_config_t config; 

	config.int_en = (uint8_t)enable;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, 1);

	if( retVal != 0 )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// setInterruptPolarity()
//
// Changes the polarity of the interrupt: active high or active low. By default
// this value is set to zero or active low. 
//
//  Parameter    Description
//  ---------    -----------------------------
//  activeHigh   Changes active state of interrupt from high to low. 
//

bool QwDevENS160::setInterruptPolarity(bool activeHigh)
{
	int32_t retVal;
	sfe_ens160_config_t config; 

	config.int_pol = (uint8_t)activeHigh;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, len);

	if( retVal != 0 )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// setInterruptDrive()
//
// Changes the pin drive of the interrupt: open drain (default) to push/pull
//
//  Parameter    Description
//  ---------    -----------------------------
//  pushPull     Changes the drive of the pin. 
//

bool QwDevENS160::setInterruptDrive(bool pushPull)
{
	int32_t retVal;
	sfe_ens160_config_t config; 

	config.int_cfg = (uint8_t)pushPull;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, len);

	if( retVal != 0 )
		return false;

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// setDataInterrupt()
//
// Routes the data ready signal to the interrupt pin.
//
//  Parameter    Description
//  ---------    -----------------------------
//  enable			 Self-explanatory: enables or disables data ready on interrupt.
//
bool QwDevENS160::setDataInterrupt(bool enable)
{
	int32_t retVal;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, (sfe_ens160_config_t)enable, 1);

	if( retVal != 0 )
		return false;

	return true; 
}


//////////////////////////////////////////////////////////////////////////////
// setGPRInterrupt()
//
// Routes the general purporse read register signal to the interrupt pin.
//
//  Parameter    Description
//  ---------    -----------------------------
//  enable			 Self-explanatory: enables or disables general purpos read interrupt.
//
bool QwDevENS160::setGPRInterrupt(bool enable)
{
	int32_t retVal;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, (sfe_ens160_config_t)enable, 1);

	if( retVal != 0 )
		return false;

	return true; 
}


//////////////////////////////////////////////////////////////////////////////
// getAppVer()
//
// Retrieves the 24 bit application version of the device.
//
//
uint32_t QwDevENS160::getAppVer()
{
	int32_t retVal;
	uint8_t tempVal[3] = {0};
	uint32_t version;

	retVal = readRegisterRegion(SFE_ENS160_GPR_READ4, &tempVal, 3);

	if( retVal != 0 )
		return 0;

	version = tempVal[0];
	version |= tempVal[1] << 8;
	version |= tempVal[2] << 16;

	return version;
}

//////////////////////////////////////////////////////////////////////////////
// setTempCompensation()
//
// The ENS160 can use temperature data to help give more accurate sensor data. 
//
//  Parameter    Description
//  ---------    -----------------------------
//  tempKelvin	 The given temperature in Kelvin 
//

bool QwDevENS160::setTempCompensation(float tempKelvin)
{
	int32_t retVal;
	sfe_ens160_temp_in_t tempVal[2] = {0};

	tempKelvin = tempKelvin * 64; // convert value - fixed equation pg. 29 of datasheet
	tempVal[0] = (tempKelvin & 0x0F);
	tempVal[1] = (tempKelvin & 0xF0) >> 8;

	retVal = writeRegisterRegion(SFE_ENS160_TEMP_IN, tempVal, 2);

	if( retVal != 0 )
		return false;

	return true; 
}



//////////////////////////////////////////////////////////////////////////////
// setRHCompensation()
//
// The ENS160 can use relative Humidiy data to help give more accurate sensor data. 
//
//  Parameter    Description
//  ---------    -----------------------------
//  humidity	   The given relative humidity. 
//
bool QwDevENS160::setRHCompensation(uint16_t humidity)
{
	int32_t retVal;
	sfe_ens160_rh_in_t tempVal[2] = {0};

	humidity = humidity * 512; // convert value - fixed equation pg. 29 in datasheet. 
	tempVal[0] = (humidity & 0x0F);
	tempVal[1] = (humidity & 0xF0) >> 8;

	retVal = writeRegisterRegion(SFE_ENS160_RH_IN, tempVal, 2);

	if( retVal != 0 )
		return false;

	return true; 
}


//////////////////////////////////////////////////////////////////////////////
// checkDataStatus()
//
// This checks the if the NEWDAT bit is high indicating that new data is ready to be read. 
// The bit is cleared when data has been read from their registers. 
//
//
bool QwDevENS160::checkDataStatus()
{
	int32_t retVal;
	sfe_ens160_device_status_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DEVICE_STATUS, &tempVal, 1);

	if( retVal != 0 )
		return false; 

	if( tempval.new_dat == 1 )
		return true;

	return false;
}


//////////////////////////////////////////////////////////////////////////////
// checkGPRStatus()
//
// This checks the if the NEWGPR bit is high indicating that there is data in the
// general purpose read registers. The bit is cleared the relevant registers have been
// read. 
//
bool QwDevENS160::checkGPRStatus()
{
	int32_t retVal;
	sfe_ens160_device_status_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DEVICE_STATUS, &tempVal, 1);

	if( retVal != 0 )
		return false; 

	if( tempval.new_gpr == 1 )
		return true;

	return false;
}


//////////////////////////////////////////////////////////////////////////////
// getFlags()
//
// This checks the status "flags" of the device (0-3).
//
uint8_t QwDevENS160::getFlags()
{
	int32_t retVal;
	sfe_ens160_device_status_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DEVICE_STATUS, &tempVal, 1);

	if( retVal != 0 )
		return 0xFF; // Change to general error

	switch( tempVal.validity_flag )
	{
		case 0: // Normal operation
			return 0;
			break;
		case 1: // Warm-up phase
			return 1;
			break;
		case 2: // Initial Start-Up Phase
			return 2;
			break;
		case 3: // Invalid Output
			return 3;
			break;
		default:
			return 0xFF;
	}
}



//////////////////////////////////////////////////////////////////////////////
// checkOperationMode()
//
// Checks the bit that indicates if an operation mode is running i.e. the device is not off. 
//
bool QwDevENS160::checkOperationMode()
{
	int32_t retVal;
	sfe_ens160_device_status_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DEVICE_STATUS, &tempVal, 1);

	if( retVal != 0 )
		return false; 

	if( tempval.new_stat_as == 1 )
		return true;

	return false;
}


//////////////////////////////////////////////////////////////////////////////
// getOperationError()
//
// Checks the bit that indicates if an invalid operating mode has been selected. 
//
bool QwDevENS160::getOperationError()
{
	int32_t retVal;
	sfe_ens160_device_status_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DEVICE_STATUS, &tempVal, 1);

	if( retVal != 0 )
		return false; 

	if( tempval.new_stat_er == 1 )
		return true;

	return false;
}



//////////////////////////////////////////////////////////////////////////////
// getAQI()
//
// This reports the calculated Air Quality Index according to UBA which is a value between 1-5. 
// The AQI-UBA is a guideline developed by the German Federal Environmental Agency and is widely 
// referenced and adopted by many countries and organizations. 
//
// 1 - Excellent, 2 - Good, 3 - Moderate, 4 - Poor, 5 - Unhealthy. 
//
uint8_t QwDevENS160::getAQI()
{
	int32_t retVal;
	sfe_ens160_data_aqi_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_AQI, &tempVal, 1);

	if( retVal != 0 )
		return 0;

	return sfe_ens160_data_aqi_t.aqi_uba;
}

//////////////////////////////////////////////////////////////////////////////
// getTVOC()
//
// This reports the Total Volatile Organic Compounds in ppb (parts per billion)
//
//
uint16_t QwDevENS160::getTVOC()
{
	int32_t retVal;
	uint16_t tvoc; 
	sfe_ens160_data_tvoc_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_TVOC, tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	tvoc = tempVal[0];
	tvoc |= (tempVal[1] & 0xF0) << 8;

	return tvoc;
}



//////////////////////////////////////////////////////////////////////////////
// getETOH()
//
// This reports the ehtanol concentration in ppb (parts per billion). According to 
// the datasheet this is a "virtual mirror" of the ethanol-calibrated TVOC register, 
// which is why they share the same register. 
//
uint16_t QwDevENS160::getETOH()
{
	int32_t retVal;
	uint16_t ethanol; 
	sfe_ens160_data_eco2_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_ETOH, tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	ethanol = tempVal[0];
	ethanol |= (tempVal[1] & 0xF0) << 8;

	return ethanol;
}


//////////////////////////////////////////////////////////////////////////////
// getECO2()
//
// This reports the CO2 concentration in ppm (parts per million) based on the detected VOCs and hydrogen. 
//
uint16_t QwDevENS160::getECO2()
{
	int32_t retVal;
	uint16_t eco; 
	sfe_ens160_data_eco2_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_ECO2, tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	eco = tempVal[0];
	eco |= (tempVal[1] & 0xF0) << 8;

	return tvoc;
}


//////////////////////////////////////////////////////////////////////////////
// getTempKelvin()
//
// This reports the temperature compensation value given to the sensor in Kelvin.
//
float QwDevENS160::getTempKelvin()
{
	int32_t retVal;
	float temperature; 
	sfe_ens160_data_t_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_T, tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	temperature = (float)tempVal[0];
	temperature |= (float)((tempVal[1] & 0xF0) << 8);

	temperature = temperature/64; // Formula as described on pg. 32 of datasheet.

	return temperature;
}


//////////////////////////////////////////////////////////////////////////////
// getTempCelsius()
//
// This reports the temperature compensation value given to the sensor in Celsius.
//
float QwDevENS160::getTempCelsius()
{
	float temperature; 

	temperature = getTempKelvin();

	return (temperature - 273.15);
}


//////////////////////////////////////////////////////////////////////////////
// getRH()
//
// This reports the relative humidity compensation value given to the sensor. 
//
float QwDevENS160::getRH()
{
	int32_t retVal;
	uint16_t rh; 
	sfe_ens160_rh_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_RH, tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	rh = tempVal[0];
	rh |= (tempVal[1] & 0xF0) << 8;

	temperature = temperature/512; // Formula as described on pg. 33 of datasheet.

	return temperature;
}
