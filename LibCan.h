/*!
 * \file 	LibCan2.h
 * \brief 	Le fichier de définition des classes LibCan2515 et LibCanProt
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#ifndef _LIB_CAN_H_
#define _LIB_CAN_H_

#define CAN_2515
#include "mcp2515_can.h"

#define DEFAULT_CAN_ID  0x70

/// \class LibCan2515
/// \brief La librairie LibCan2515 permet de gérer un bus CAN en émission et en réception
class LibCan2515
{
  public:

		/// \details Permet dinitialiser la librairie en utilisant la configuration SPI par défaut
		LibCan2515(
			unsigned char csPin ///< Pin associée au signal CS
		);

		/// \details Permet dinitialiser la librairie en associant des pins non standard pour le SPI
		LibCan2515(
			unsigned char sckPin,  ///< Pin associée au signal SCK
			unsigned char misoPin, ///< Pin associée au signal MISO
			unsigned char mosiPin, ///< Pin associée au signal MOSI
			unsigned char csPin    ///< Pin associée au signal CS
		);

		/// \details Permet de valider l'interface CAN
		/// \return Retourne true si le bus CAN est prêt et false sinon 
		bool begin(
			unsigned char scanId   = DEFAULT_CAN_ID, ///< Adresse de l'interface CAN
			unsigned int  speedSet = CAN_125KBPS,    ///< Vitesse d'échange sur le bus can
			unsigned char clockSet = MCP_8MHz        ///< Desciption du role du parametre 1
		);

		/// \details Permet de lire un message
		/// \return Retourne la taille du message reçu et 0 si aucun message reçu 
		unsigned char getMessage(
			unsigned char * p_buf ///< Pointeur vers le message ou retourner le message
		);

		/// \details Permet d'envoyer' un message
		/// \return Retourne la taille du message reçu et 0 si aucun message reçu 
		void sendMessage(
			unsigned char * p_buf, ///< Pointeur vers le message à envoyer
			unsigned char len      ///< Longueur du message
		);

		/// \details Permet de valider l'affichage de message de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		);

  private:

    bool m_debug;
		unsigned char m_canId, m_sckPin, m_misoPin, m_mosiPin, m_csPin;
    unsigned char m_clockSet;
		unsigned int  m_speedSet;
   
	 	mcp2515_can can;

		/// \details Procédure d'initialisation de la librairie
		void init(void);

		/// \details Affichage d'un message reçu
		void displayMessage(
			unsigned char * p_buf, ///< Pointeur vers le message à afficher
			unsigned char   len    ///< Taille du message
		);

  protected:
};




/// \class LibCanProt
/// \brief La librairie LibCanProt permet de gérer le protocole utilisé entre le poste de commande du robot et le robot
/// \brief Elle s'appuie sur la librairie \ref LibCan2515

#include "LibMoteur.h"

#define BUS_CAN_MESSAGE_MAX_SIZE 64

class LibCanProt
{
  public:
		/// \details 
		LibCanProt(
			// unsigned char csPin ///< Pin associée au signal CS
		);

    /// \details Permet de donner les références du driver du bus CAN
    void setCanBusDriver(
			LibCan2515 * p_canBus ///< Pointeur vers le gestionnaire du bus CAN
		) { mp_canBus = p_canBus; }

		/// \details Permet de donner les références des moteurs
		/// \details Il est possible de gérer 2 moteurs
		void setMoteurDriver(
			unsigned char moteur, ///< Moteur à configurer (0-1)
			LibMoteur* p_moteur   ///< Pointeur vers le gestionnaire de moteur
		);

		/// \details Permet de valider l'interface CAN et le gestion du protocole
		/// \return Retourne true si le bus CAN est prêt et false sinon 
		bool begin(	void );

		/// \details Permet de traiter l'arrivée d'un éventuel message
		/// \return Retourne true si un message a été traité et flase sinon
		bool gestionMessage(
			// unsigned char csPin ///< Pin associée au signal CS
		);

		void sendSetPinDigital(unsigned char pin, unsigned char status);
		void sendSetPinAnalog(unsigned char pin, unsigned short val);
		void sendSetMoteur(unsigned char moteur, char vitesseGauche, char vitesseDroite);
		void sendDisplayString(char* message, unsigned char len);

		/// \details Permet de valider l'affichage de message de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		);

  private:

    bool m_debug;
    bool m_begin;
		unsigned char m_bufferCan[BUS_CAN_MESSAGE_MAX_SIZE];

		LibCan2515 * mp_canBus  = NULL;
		LibMoteur  * mp_moteur1 = NULL;
		LibMoteur  * mp_moteur2 = NULL;

    void setPinDigital(void);
 		void setPinAnalog (void); 
    void setMoteur    (unsigned char moteur);
    void displayString(void);

		/// \details Affichage du type de message reçu
		void displayMessageRecu(
			unsigned char cmd  ///< Code de la commande reçue
		);

		/// \details Affichage du type de message reçu
		void displayMessageEnvoye(
			unsigned char cmd  ///< Code de la commande envoyée
		);

		/// \details Affichage du libellé de la commade
		void displayMessageString(
			unsigned char cmd  ///< Code de la commande
		);

  protected:
};

#endif