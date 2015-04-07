/**
\mainpage Classe CI2C
\author Simon Moinet
\date Mars 2015

Cette classe CI2C permet de lire/écrire les valeurs de registres des 
esclaves d'un bus I2C

\file CI2C.h
\brief Contient la declaration de la CI2C
*/

#ifndef CI2C_H
#define CI2C_H

#include <fcntl.h> // O_RDWR
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <exception>
#include <string>
#include <sstream>

using namespace std;

/**
 *\class CI2C
 *\brief Cette classe CI2C permet de lire/écrire les valeurs de registres des esclaves d'un bus I2C
 */
class CI2C {
	
	public:

		/**
		 * \class ErreurWrite
		 * \brief Cette classe est une esception qui est levé lorsqu'il y a une erreur d'écriture
		 */
		class ErreurWrite : public exception {
			private:
				/// Contient le registre dans lequel on a essayé d'écrire
				int m_reg;
				/// Contient la valeur qu'on a essayer d'écrire dans le registre
				int m_data;

			public:
				/**
				 * \brief Constructeur de la classe ErreurWrite
				 *
				 * Ce contructeur initialise les attributs de la classe ErreurWrite
				 *
				 * \param[in] reg : le registre dans lequel on essaye d'écrire
				 * \param[in] data : la valeur que l'on essaye d'écrire dans le registre
				 */
				ErreurWrite(int reg, int data):m_reg(reg), m_data(data) {};

				/**
				 * \brief Methode what
				 * \return la description de l'exception avec la valeur et le registre qui ont provoqué l'erreur
				 */
				virtual const char* what() const noexcept {
					stringstream temp;
					temp << "Erreur d'écriture de la valeur 0x" << hex << m_data << " dans le registre 0x" << m_reg;
					return temp.str().c_str();
				};
		};

		/**
		 * \class ErreurRead
		 * \brief Cette classe est une exception qui est levé lorsqu'il y a un erreur de lecture
		 */
		class ErreurRead : public exception {
			private:
				/// Contient le registre que l'on a essayé de lire
				int m_reg;

			public:
				/**
				 * \brief Constructeur de la classe ErreurRead
				 *
				 * Ce contructeur initialise les attributs de la classe ErreurWrite
				 *
				 * \param[in] reg : le registre que l'on a essayé de lire
				 */
				ErreurRead(int reg):m_reg(reg) {};

				/**
				 * \brief Methode what
				 *
				 * \return la description de l'esception avec la valeur du registre que l'on a essayé  de lire
				 */
				virtual const char* what() const noexcept {
					stringstream temp;
					temp << "Erreur de lecture du registre 0x" << hex << m_reg;
					return temp.str().c_str();
				};
		};

		/**
		 * \class ErreurOpenDev
		 * \brief exception lorsqu'on essaye d'ouvrir le dev spécifié
		 */
		class ErreurOpenDev : public exception {
			private:
				/// contient le nom du device que l'on essaye d'ouvrir
				string m_devName;

			public:
				/**
				 * \brief Contructeur de la classe ErreurOpenDev
				 *
				 * Ce contructeur initialise les attributs de la classe ErreurOpenDev
				 * 
				 * \param[in] devName : le nom du device que l'on a essayé d'ouvrir
				 */
				ErreurOpenDev(string devName):m_devName(devName) {};

				/**
				 * \brief Methode what
				 * \return la description de l'exception avec le nom du device que l'on a essayé d'ouvrir
				 */
				virtual const char* what() const noexcept {
					string temp = "Impossible d'ouvrir le bus i2c : " + m_devName;
					return temp.c_str();
				};
		};

		/**
		 * \class ErreurSetAddrSlave
		 * \brief exception definition de l'esclave
		 */
		class ErreurSetAddrSlave : public exception {
			private:
				/// Contient l'addresse de l'esclave avec lequel on veux communiquer
				int m_addr;

			public:
				/**
				 * \brief Constructeur de la classe ErreurSetAddrSlave
				 *
				 * Ce contructeur initialise les attributs de la classe ErreurSetAddrSlave
				 *
				 * \param[in] addr : l'adresse de l'esclave avec lequel on veux communiquer
				 */
				ErreurSetAddrSlave(int addr):m_addr(addr) {};

				/**
				 * \brief Methode what
				 * \return la description de l'exception
				 */
				virtual const char* what() const noexcept {
					stringstream temp;
					temp << "Impossible de definir l'esclave avec lequel on veux communiquer. L'eclave est indisponible ou il n'y a pas d'esclave à l'adresse " << hex << m_addr;
					return temp.str().c_str();
				};
		};

		/**
		 * \class ErreurDevNotDefine
		 * \brief exception device non défini
		 */
		class ErreurDevNotDefine : public exception {
			public:
				/**
				 * \brief Methode what
				 * \return la description de l'exception
				 */
				virtual const char* what() const noexcept {
					return "Erreur, le device I2C n'est pas défini";
				};
		};

		/**
		 * \class ErreurSlaveNotDefine
		 * \brief exception adresse de l'esclave non défini
		 */
		class ErreurSlaveNotDefine : public exception {
			public:
				/**
				 * \brief Methode what
				 * \return la description de l'exception
				 */
				virtual const char* what() const noexcept {
					return "Erreur, L'esclave n'est pas défini";
				};
		};

	private:
		/// Contient les informations du bus I2C
		int dev;
		/// Bool qui permet de savoir si le dev est défini
		bool isSetDev; 
		/// Bool qui permet de savoir sur l'esclave est défini
		bool isSetSlave;

		/**
		 * \brief Methode isSetInterface
		 *
		 * Cette méthode permet de verifier si le dev et l'esclave sont bien défini
		 * 
		 * \exception StringIndexOutOfRangeException
		 *             if index is not between
		 *             <code>0</code> and
		 *             <code>length() - 1</code>.
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si le dev n'est pas défini, leve une exception de type Erreur::TYPE_ERREUR::DEV_DEF
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si l'esclave n'est pas défini, leve une exception de type Erreur::TYPE_ERREUR::SLAVE_DEF
		 */
		void isSetInterface();

	public:
		/**
		 * \brief Constructeur de la classe CI2C
		 *
		 * Ce constructeur initialise les attributs de la classe.
		 */
		CI2C();

		/**
		 * \brief Destructeur de la classe CI2C
		 */
		~CI2C();

		/**
		 * \brief Methode setupInterface 
		 *
		 * Initialisation de l'interface I2C et de son esclave.
		 * 
		 * \param[in] dev : le nom de l'interface I2C
		 * \param[in] addr_slave : l'addresse de l'esclave sur le bus I2C
		 *
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si il est impossible de definir le device du bus I2C. Leve une exception de type Erreur::TYPE_ERREUR::DEV_DEF
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si la spécification de l'adresse de l'esclave avec lequel on veux communiquer à échoué. Leve une exception de type Erreur::TYPE_ERREUR::SLAVE_DEF 
		 */
		void setupInterface(string dev, __u8 addr_slave);

		/**
		 * \brief Methode setSlaveAddr
		 *
		 * Paramétrage de la nouvelle addresse de l'esclave sur le bus I2C
		 * 
		 * \param[in] __u8 addr_slave : la nouvelle addresse de l'esclave
		 *
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si le dev n'est pas défini, leve une exception de type Erreur::TYPE_ERREUR::DEV_DEF
		 * \exception Erreur(TYPE_ERREUR erreur, string const& phrase)
		 * Si la spécification de l'adresse de l'esclave avec lequel on veux communiquer à échoué. Leve une exception de type Erreur::TYPE_ERREUR::SLAVE_DEF 
		 */		
		void setSlaveAddr(__u8 addr_slave);
		
		/**
		 * \brief Methode readByte 
		 * 
		 * Lecture du registre sur un octet.
		 * Par default la valeur est fixé à zéro dans le cas d'un esclave sans registre
		 * 
		 * \param[in] reg : le registre que l'on veux lire
		 * \return Un int contenant l'octet du registre lu
		 */
		int readByte(__u8 reg = 0);
		
		/**
		 * \brief Methode readWord 
		 *
		 * Lecture du registre sur deux octets.
		 * Par default la valeur est ficé à zéro dans le cas d'un esclave sans registre
		 *
		 * \param[in] reg : le registre que l'on veux lire
		 * \return Un int contenant les deux octets du registre lu
		 */
		int readWord(__u8 reg = 0);
		
		/**
		 * \brief Methode writeByte
		 * 
		 * Ecriture dans un registre sur un octet
		 *
		 * \param[in] reg : le registre où on veux écrire
		 * \param[in] data : l'octet que l'on veux ecrire dans le registre
		 * \return true si l'écriture a réussi, false sinon
		 */
		void writeByte(__u8 reg, __u8 data);
		
		/**
		 * \brief Methode writeWord
		 * 
		 * Ecriture dans un registre sur deux octets
		 *
		 * \param[in] reg : le registre où on veux écrire
		 * \param[in] data : les deux otets que l'on veux ecrire dans le registre
		 * \return true si l'écriture a réussi, false sinon
		 */
		void writeWord(__u8 reg, __u16 data);
};


#endif // CI2C_H
