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
		throw Erreur(Erreur::TYPE_ERREUR::DEV_DEF, "ERREUR : Impossible d'ouvrir le bus I2C -> " + dev);

	isSetDev = true;

	if(ioctl(this->dev, I2C_SLAVE, addr_slave) < 0)
		throw Erreur(Erreur::TYPE_ERREUR::SLAVE_DEF, "ERREUR : Impossible de definir l'esclave avec lequel on veux communiquer. L'eclave est indisponible ou il n'y a pas d'esclave à l'adresse " + to_string(addr_slave));

	isSetSlave = true;
}

// Methode setSlaveAddr
void CI2C::setSlaveAddr(__u8 addr_slave)
{
	if(ioctl(this->dev, I2C_SLAVE, addr_slave) < 0)
	{
		throw Erreur(Erreur::TYPE_ERREUR::SLAVE_DEF, "ERREUR : Impossible de definir l'esclave avec lequel on veux communiquer. L'eclave est indisponible ou il n'y a pas d'esclave à l'adresse " + to_string(addr_slave));
		isSetSlave = false;
	}

	isSetSlave = true;
}

void CI2C::isSetInterface()
{
	if(!isSetDev)
		throw Erreur(Erreur::TYPE_ERREUR::DEV_NO_DEF, "ERREUR : Le device n'est pas défini");

	if(!isSetSlave)
		throw Erreur(Erreur::TYPE_ERREUR::SLAVE_NO_DEF, "ERREUR : L'esclave n'est pas défini");
}

// Methode readByte
int CI2C::readByte(__u8 reg)
{
	__s32 res = 0;

	isSetInterface();	

	res = i2c_smbus_read_byte_data(this->dev, reg);
	if(res < 0)
		throw Erreur(Erreur::TYPE_ERREUR::READ, "ERREUR : Impossible de lire la valeur du registre " + to_string(reg));

	return static_cast<int>(res);
}

// Methode readWord
int CI2C::readWord(__u8 reg)
{
	__s32 res = 0;

	isSetInterface();

	res = i2c_smbus_read_word_data(this->dev, reg);
	if(res < 0)
		throw Erreur(Erreur::TYPE_ERREUR::READ, "ERREUR : Impossible de lire la valeur du registre " + to_string(reg));

	return static_cast<int>(res);
}

// Methode writeByte
void CI2C::writeByte(__u8 reg, __u8 data)
{
	__s32 error = 0;

	isSetInterface();

	error = i2c_smbus_write_byte_data(this->dev, reg, data);
	if(error < 0)
		throw Erreur(Erreur::TYPE_ERREUR::WRITE, "ERREUR : Impossible d'écrire la valeur " + to_string(data) + " dans le registre " + to_string(reg));
}

// Methode writeWord
void CI2C::writeWord(__u8 reg, __u16 data)
{
	__s32 error = 0;

	isSetInterface();

	error = i2c_smbus_write_word_data(this->dev, reg, data);
	if(error < 0)
		throw Erreur(Erreur::TYPE_ERREUR::WRITE, "ERREUR : Impossible d'écrire la valeur " + to_string(data) + " dans le registre " + to_string(reg));
}