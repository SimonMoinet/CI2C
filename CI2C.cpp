/**
\file CI2C.cpp
\brief Contient la définition de la classe CI2C
*/

#include "CI2C.h"

// Constructeur
CI2C::CI2C()
{
	dev = 0;
	isSetDev = false;
	isSetSlave = false;
}

// Destructeur
CI2C::~CI2C()
{
}

// Methode setupInterface
void CI2C::setupInterface(string dev, __u8 addr_slave)
{
	this->dev = open(dev.c_str(), O_RDWR);
	if(this->dev < 0)
		throw ErreurOpenDev(dev);

	isSetDev = true;

	if(ioctl(this->dev, I2C_SLAVE, addr_slave) < 0)
		throw ErreurSetAddrSlave(static_cast<int>(addr_slave));

	isSetSlave = true;
}

// Methode setSlaveAddr
void CI2C::setSlaveAddr(__u8 addr_slave)
{
	if(!isSetDev)
		throw ErreurDevNotDefine();

	if(ioctl(this->dev, I2C_SLAVE, addr_slave) < 0)
	{
		throw ErreurSetAddrSlave(static_cast<int>(addr_slave));
		isSetSlave = false;
	}

	isSetSlave = true;
}

void CI2C::isSetInterface()
{
	if(!isSetDev)
		throw ErreurDevNotDefine();

	if(!isSetSlave)
		throw ErreurSlaveNotDefine();
}

// Methode readByte
int CI2C::readByte(__u8 reg)
{
	__s32 res = 0;

	isSetInterface();	

	res = i2c_smbus_read_byte_data(this->dev, reg);
	if(res < 0)
		throw ErreurRead(static_cast<int>(reg));

	return static_cast<int>(res);
}

// Methode readWord
int CI2C::readWord(__u8 reg)
{
	__s32 res = 0;

	isSetInterface();

	res = i2c_smbus_read_word_data(this->dev, reg);
	if(res < 0)
		throw ErreurRead(static_cast<int>(reg));

	return static_cast<int>(res);
}

// Methode writeByte
void CI2C::writeByte(__u8 reg, __u8 data)
{
	__s32 error = 0;

	isSetInterface();

	error = i2c_smbus_write_byte_data(this->dev, reg, data);
	if(error < 0)
		throw ErreurWrite(static_cast<int>(reg), static_cast<int>(data));
}

// Methode writeWord
void CI2C::writeWord(__u8 reg, __u16 data)
{
	__s32 error = 0;

	isSetInterface();

	error = i2c_smbus_write_word_data(this->dev, reg, data);
	if(error < 0)
		throw ErreurWrite(static_cast<int>(reg), static_cast<int>(data));
}