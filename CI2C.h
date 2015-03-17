/**
\mainpage Classe CI2C
\author Simon Moinet
\date Mars 2015

Cette classe CI2C permet de lire/écrire les valeurs de registres des 
esclaves d'un bus I2C

\file CI2C.h
\brief Contient la declaration de la CI2C

\class CI2C
\brief Cette classe CI2C permet de lire/écrire les valeurs de registres des esclaves d'un bus I2C
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
		/** \class Erreur
		 * \brief Cette classe permet de levé des exception qui corrspondent aux erreurs de la classes CI2C
		 */
		class Erreur : public exception {
			public:
				/** \enum TYPE_ERREUR
				 * Cette énumération décrit les erreurs possible de la classes CI2C
				 */
				enum class TYPE_ERREUR : int {
					WRITE, ///< echec de l'écriture dans un registre
					READ, ///< echec de la lecture d'un registre 
					DEV_DEF, ///< echec de la definition du device I2C
					SLAVE_DEF, ///< echec de la definition de l'adresse de l'esclave
					DEV_NO_DEF, ///< le device I2C n'est pas défini
					SLAVE_NO_DEF ///< l'esclave n'est pas défini
				};

			private:
				/// Contient une phrase qui correspond à l'erreur
				string m_phrase;
				/// Contient le type de l'erreur
				TYPE_ERREUR m_erreur;

			public:
				/**
				 * \brief Constructeur de la classe Erreur
				 *
				 * Ce constructeur initialise les parametres de la classe.
				 *
				 * \param[in] erreur : Le type d'erreur qui a été levé
				 * \param[in] phrase : Une phrase qui contient un texte sur l'erreur levé
				 */
				Erreur(TYPE_ERREUR erreur, string const& phrase) noexcept
				{
					m_erreur = erreur;
					m_phrase = phrase;
				}

				/**
				 * \brief Destructeur de la classe Erreur
				 */
				virtual ~Erreur() noexcept;

				/**
				 * \brief Methode what
				 */
				virtual const char* what() const noexcept
				{
					return m_phrase.c_str();
				}

				/**
				 * \brief Methode getTypeErreur
				 */
				TYPE_ERREUR getTypeErreur() noexcept
				{
					return m_erreur;
				}
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
