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


bool QwDevENS160::getDataStatus()
{
	int32_t retVal;
	sfe_ens160_config_t tempVal;

	retVal = readRegisterRegion(SFE_ENS160_, &tempVal, 1);

	if( retVal != 0 )
		return false;

	if( tempVal.int_dat == 0 )
		return false

	return true; 
}

bool QwDevENS160::getGPRStatus()
{
	int32_t retVal;
	sfe_ens160_config_t tempVal;

	retVal = readRegisterRegion(SFE_ENS160_, &tempVal, 1);

	if( retVal != 0 )
		return false;

	if( tempVal.int_gpr == 0 )
		return false

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
