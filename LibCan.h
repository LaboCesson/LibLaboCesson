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
#define CAN_MAGIC_TAG 0xC7

/// \class LibCan2515
/// \brief La librairie LibCan2515 permet de gérer un bus CAN en émission et en réception
class LibCan2515
{
  public:

		/// \details Permet d'initialiser la librairie en utilisant la configuration SPI par défaut
		LibCan2515(
			unsigned char csPin ///< Pin associée au signal CS
		);

		/// \details Permet d'initialiser la librairie en associant des pins non standard pour le SPI
		LibCan2515(
			unsigned char sckPin,  ///< Pin associée au signal SCK
			unsigned char misoPin, ///< Pin associée au signal MISO
			unsigned char mosiPin, ///< Pin associée au signal MOSI
			unsigned char csPin    ///< Pin associée au signal CS
		);

		/// \details Permet de valider l'interface CAN
		/// \return Retourne **true** si le bus CAN est prêt et **false** sinon 
		bool begin(
			unsigned char scanId   = DEFAULT_CAN_ID, ///< Adresse de l'interface CAN
			unsigned int  speedSet = CAN_125KBPS,    ///< Vitesse d'échange sur le bus can
			unsigned char clockSet = MCP_8MHz        ///< Desciption du role du parametre 1
		);

		/// \details Permet de lire un message
		/// \return Retourne la taille du message reçu et 0 si aucun message n'est reçu 
		unsigned char getMessage(
			unsigned char * p_buf ///< Pointeur vers le message où retourner le message
		);

		/// \details Permet d'envoyer un message
		void sendMessage(
			unsigned char * p_buf, ///< Pointeur vers le message à envoyer
			unsigned char len      ///< Longueur du message
		);

		/// \details Permet de valider l'affichage de messages de debug
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
#include "LibColor.h"
#include "LibGpio.h"

#define BUS_CAN_MESSAGE_MAX_SIZE 64
#define BUS_CAN_TIME_OUT         100 // Durée d'attente max dune réponse (par unité de 10ms)

class LibCanProt
{
  public:
		/// \details 
		LibCanProt(void);

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

		/// \details Permet de donner les références du driver du détecteur de couleur
		void setColorDriver(
			LibTcs3472* p_color ///< Pointeur vers le gestionnaire du détecteur de couleur
		) { mp_color = p_color; }

		/// \details Permet de donner les références du driver du gestionnaire de GPIO
		void setGpioDriver(
			LibGpio* p_gpio ///< Pointeur vers le gestionnaire de GPIO
		) { mp_gpio = p_gpio; }

		/// \details Permet de valider l'interface CAN et le gestion du protocole
		/// \return Retourne true si le bus CAN est prêt et false sinon 
		bool begin(	void );

		/// \details Permet de traiter l'arrivée d'un éventuel message
		/// \return Retourne true si un message a été traité et false sinon
		bool gestionMessage(
			// unsigned char csPin ///< Pin associée au signal CS
		);


		/// \details Permet de configurer l'état d'un pin de l'équipement distant
		void sendSetPinDigital(unsigned char pin, unsigned char status);

		/// \details Permet de configurer la valeur d'un pin analogique (0-1023) de l'équipement distant
		void sendSetPinAnalog(unsigned char pin, unsigned short val);

		/// \details Permet de configurer la vitesse des moteurs associés au moteur 1
		void sendSetMoteur1(char vitesseGauche, char vitesseDroite);

		/// \details Permet de configurer la vitesse des moteurs associés au moteur 2
		void sendSetMoteur2(char vitesseGauche, char vitesseDroite);

		/// \details Permet de d'afficher un message sur l'afficheur de l'équipement distant
		void sendDisplayString(char* message, unsigned char len);

		/// \details Permet de lire l'état d'un pin de l'équipement distant
		/// \return  Retourne l'état 0/1 ou -1 si aucune réponse n'a été reçu
		int sendGetPinDigital(unsigned char pin);

		/// \details Permet de lire la valeur analogique d'un pin de l'équipement distant
		/// \return  Retourne la valeur lue ou -1 si aucune réponse n'a été reçu
		int sendGetPinAnalog(unsigned char pin);

		/// \details Permet de lire les couleurs lues par l'équipement distant
		/// \return  Retourne les couleurs ou -1 si aucune réponse n'a été reçu <BR>
		/// Les couleurs sont codées par groupe de 2 bits avec les valeurs définies par \ref t_robot_color
		int sendGetColor(unsigned char nbColor); ///< Nombre de couleurs à lire

		/// \details Permet de configurer l'angle de rotation d'un servoMoteur désigné par son numéro de pin
		void sendSetGpioPwm(unsigned char gpio, char angle);

		/// \details Permet de vider les messages reçus
		/// \details A utiliser avant l'envoi d'une commande avec une réponse
		/// pour s'assurer qu'une ancienne réponse après time_out serait prise en compte
		/// \details A utiliser également à la fin du setup
		void purgeMessageRecu(void);

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
		LibTcs3472 * mp_color   = NULL;
		LibGpio    * mp_gpio    = NULL;

		void setPinDigital(void);
		void getPinDigital(void);
		void setPinAnalog (void);
		void getPinAnalog (void);
		void setMoteur    (unsigned char moteur);
		void displayString(void);
		void getColor     (void);
		void setGpioPwm   (void);

		void returnGetPinDigital(unsigned char pin, unsigned char status);
		void returnGetPinAnalog(unsigned char pin, unsigned char value);
		void returnGetColor(unsigned char color);

		//========== Routines de gestion des messages ========

		/// \details Envoi le message présent dans le buffer
		void envoiMessage(void);

		///// \details Envoi la réponse présente dans le buffer
		//void envoiReponse(void);

		/// \details Attends une réponse avec un time-out
		/// \return  Retourne true si un message est présent dans le buffer m_bufferCan et false sinon
		bool attendReponse(
			int timeOut  ///< Time-out exprimé en nombre 10ms à attendre
		);


		//========== Routines d'affichage de messages de debug ========

		void displayPinInfo(
			unsigned char pin,  ///< Numéro du pin
			unsigned int  val   ///< Valeur associé au pin
		);

		void displayVitessesInfo(
			char vitesseGauche,  ///< Vitesse moteur Gauche
			char vitesseDroite   ///< Vitesse moteur Droit
		);

		void displayColorInfo(
			unsigned int color   ///< Description des couleurs
		);

		void displayGpioAngleInfo(
			unsigned char gpio,  ///< Index du gpio
			char          angle  ///< Angle associé au pin
		);

		/// \details Affichage du libellé de la commande
		void displayMessageString(
			unsigned char cmd  ///< Code de la commande
		);

  protected:
};

#endif