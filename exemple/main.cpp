#include <iostream>
#include <stdlib.h>

#include "../../Classes/CI2C/CI2C.h"

using namespace std;

int main()
{
	string filename = "/dev/i2c-1";
	int res = 0;
	__u8 addr_slave = 0x4d;
	CI2C i2c;

	system("clear");
	cout << "DEBUT DU PROG :" << endl;
	cout << "###############" << endl << endl;

	try {
		cout << "-> Setup interface (i2c : /dev/i2c-1, addr_slave : 0x4d)";
		i2c.setupInterface(filename, addr_slave);
		cout << " : OK !" << endl << endl;

		cout << "Lecture :" << endl;
		cout << "-> Lecture d'un octet au registre 0x00 à l'adresse 0x4d";
		cout << " = val : 0x" << hex << i2c.readByte() << endl;

		cout << "-> Lecture de deux octets au registre 0x00 à l'adresse 0x4d";
		cout << " = val : 0x" << hex << i2c.readWord() << endl;

		cout << "-> Setup slave (addr_slave : 0x68)";
		i2c.setSlaveAddr(0x68);
		cout << " : OK !" << endl << endl;

		cout << "Ecriture :" << endl;
		cout << "-> Lecture d'un octet au registre 0x00 à l'adresse 0x68";
		cout << " = val : 0x" << hex << i2c.readByte() << endl;

		cout << "-> Ecriture de la valeur 0x00 sur le registre 0x00 à l'adresse 0x68";
		i2c.writeByte(0x00, 0x00);
		cout << " : OK !" << endl;

		cout << "-> Lecture d'un octet au registre 0x00 à l'adresse 0x68";
		cout << " = val : 0x" << hex << i2c.readByte() << endl;

		cout << "-> Lecture de deux octets au registre 0x00 à l'adresse 0x68";
		cout << " = val : 0x" << hex << i2c.readWord() << endl;

		cout << "-> Ecriture de la valeur 0x0000 sur le registre 0x00 à l'adresse 0x68";
		i2c.writeWord(0x00, 0x0000);
		cout << " : OK !" << endl;
		
		cout << "-> Lecture de deux octets au registre 0x00 à l'adresse 0x68";
		cout << " = val : 0x" << hex << i2c.readWord() << endl;
	}
	catch(const exception& e) {
		std::cerr << e.what() << endl;
	}

	return 0;
}