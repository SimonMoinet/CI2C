/**
\mainpage Classe CI2C
\author Simon Moinet
\date Mars 2015

Cette classe CI2C permet de lire/écrire les valeurs de registres des 
esclaves d'un bus I2C

\file CI2C.cpp
\class CI2C
\biref Cette classe CI2C permet de lire/écrire les valeurs de registres des esclaves d'un bus I2C
*/

#ifndef CI2C_H
#define CI2C_H

#include <fcntl.h> // O_RDWR
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <exception>
#include <string>

using namespace std;

class CI2C {
	
	public:
		class Erreur : public exception {
			public:
				enum class TYPE_ERREUR : int
				{WRITE, READ, DEV_DEF, SLAVE_DEF, DEV_NO_DEF, SLAVE_NO_DEF};

			private:
				string m_phrase;
				TYPE_ERREUR m_erreur;

			public:
				Erreur(TYPE_ERREUR erreur, string const& phrase) noexcept;
				virtual ~Erreur() noexcept;
				virtual const char* what() const noexcept
				{
					return m_phrase.c_str();
				}
				TYPE_ERREUR getTypeErreur() noexcept
				{
					return m_erreur;
				}
		};

	private:
		int dev; ///<Contient les informations du bus I2C
		bool isSetDev;
		bool isSetSlave;

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
		 */
		void setupInterface(string dev, __u8 addr_slave);

		/**
		 * \brief Methode setSlaveAddr
		 *
		 * Paramétrage de la nouvelle addresse de l'esclave sur le bus I2C
		 * 
		 * \param[in] __u8 addr_slave : la nouvelle addresse de l'esclave
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
