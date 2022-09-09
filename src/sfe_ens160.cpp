#include "sfe_ens160.h"

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

int32_t QwDevENS160::writeRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->writeRegisterRegion(_i2cAddress, offset, data, length);
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

int32_t QwDevENS160::readRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->readRegisterRegion(_i2cAddress, offset, data, length);
}

int32_t QwDevENS160::getUniqueID()
{
	int32_t retVal;
	uint8_t tempVal[2] = {0}; 
	uint16_t id; 

	retVal = readRegisterRegion(_i2cAddress, 0x00, tempVal, 2);

	
	id = tempVal[0];
	id |= tempVal[1] << 8;

	if( retVal != 0 )
		return -1;

	return id; 

}


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


bool QwDevENS160::setInterruptEnable(bool enable)
{
	int32_t retVal;

	sfe_ens160_config_t config; 

	if( enable == true )
		config.int_en = 1;
	else
		config.int_en = 1;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, 1);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevENS160::setInterruptPolarity(uint8_t level)
{
	int32_t retVal;

	sfe_ens160_config_t config; 

	if( enable == 1 )
		config.int_pol = 1;
	else
		config.int_pol = 0;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, len);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevENS160::setInterruptDrive(uint8_t push_open)
{
	int32_t retVal;

	sfe_ens160_config_t config; 

	if( enable == 1 )
		config.int_cfg = 1;
	else
		config.int_cfg = 0;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, config, len);

	if( retVal != 0 )
		return false;

	return true;
}


bool QwDevENS160::setDataInterrupt(bool enable)
{
	int32_t retVal;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, (sfe_ens160_config_t)enable, 1);

	if( retVal != 0 )
		return false;

	return true; 
}

bool QwDevENS160::setGPRInterrupt(bool enable)
{
	int32_t retVal;

	retVal = writeRegisterRegion(SFE_ENS160_CONFIG, (sfe_ens160_config_t)enable, 1);

	if( retVal != 0 )
		return false;

	return true; 
}

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

bool QwDevENS160::getError()
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


// Reports the calculated Air Quality Index according to UBA.
uint8_t QwDevENS160::getAQI()
{
	int32_t retVal;
	sfe_ens160_data_aqi_t tempVal; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_AQI, &tempVal, 1);

	if( retVal != 0 )
		return 0;

	return sfe_ens160_data_aqi_t.aqi_uba;
}

uint16_t QwDevENS160::getTVOC()
{
	int32_t retVal;
	uint16_t tvoc; 
	sfe_ens160_data_tvoc_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_TVOC, &tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	tvoc = tempVal[0];
	tvoc |= (tempVal[1] & 0xF0) << 8;

	return tvoc;
}


uint16_t QwDevENS160::getETOH()
{
	int32_t retVal;
	uint16_t ethanol; 
	sfe_ens160_data_eco2_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_ETOH, &tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	ethanol = tempVal[0];
	ethanol |= (tempVal[1] & 0xF0) << 8;

	return ethanol;
}

uint16_t QwDevENS160::getECO2()
{
	int32_t retVal;
	uint16_t eco; 
	sfe_ens160_data_eco2_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_ECO2, &tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	eco = tempVal[0];
	eco |= (tempVal[1] & 0xF0) << 8;

	return tvoc;
}

float QwDevENS160::getTempKelvin()
{
	int32_t retVal;
	float temperature; 
	sfe_ens160_data_t_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_T, &tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	temperature = (float)tempVal[0];
	temperature |= (float)((tempVal[1] & 0xF0) << 8);

	temperature = temperature/64; // Formula as described on pg. 32 of datasheet.

	return temperature;
}

float QwDevENS160::getTempCelsius()
{
	float temperature; 

	temperature = getTempKelvin();

	return (temperature - 273.15);
}


float QwDevENS160::getRH()
{
	int32_t retVal;
	uint16_t rh; 
	sfe_ens160_rh_t tempVal[2] = {0}; 

	retVal = readRegisterRegion(SFE_ENS160_DATA_RH, &tempVal, 2);

	if( retVal != 0 )
		return 0;
	
	rh = tempVal[0];
	rh |= (tempVal[1] & 0xF0) << 8;

	temperature = temperature/512; // Formula as described on pg. 33 of datasheet.

	return temperature;
}
