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
/// \details Librairies requises :
/// \details  - **Seeed_Arduino_CAN** par seeed studio

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


/// \class LibCanProtCommon
/// \brief La librairie LibCanProtCommon contient des fonctions communes à l'émission et à la réception

#define BUS_CAN_MESSAGE_MAX_SIZE 64
#define BUS_CAN_TIME_OUT         100 // Durée d'attente max dune réponse (par unité de 10ms)

class LibCanProtCommon
{
public:

	void setDebug(bool debug) { m_debug = debug; }; ///< si true les messages de debug sont affichése les messages de debug sont affichés

	///// \details Envoi le message présent dans le buffer
	//void envoiMessage(void);

	void displayAck(
		unsigned char cmd ///< Commande à acquitter
	);

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
		unsigned char angle  ///< Angle associé au pin (0-90/180)
	);

	void displayRelayStatus(
		unsigned char relay,  ///< Index du relay (1-8)
		bool          status  ///< nouveau status (LOW=ouvert HIGH=Fermé)
	);

	/// \details Affichage du libellé de la commande
	void displayMessageString(
		unsigned char cmd  ///< Code de la commande
	);

private:

	bool m_debug = false;

protected:
};



/// \class LibCanProtSend
/// \brief La librairie LibCanProtSend permet de gérer le protocole utilisé entre le poste de commande du robot et le robot
/// \brief Elle s'appuie sur la librairie \ref LibCan2515

class LibCanProtSend
{
  public:
		/// \details 
		LibCanProtSend(void);

    /// \details Permet de donner les références du driver du bus CAN
    void setCanBusDriver(
			LibCan2515 * p_canBus ///< Pointeur vers le gestionnaire du bus CAN
		) { mp_canBus = p_canBus; }


		/// \details Permet de valider l'interface CAN et le gestion du protocole
		/// \return Retourne true si le bus CAN est prêt et false sinon 
		bool begin(	void );

		/// \details Permet d'envoyer un message de test vers l'équipement distant
		/// \return  Retourne true si le test est OK et false sinon
		bool sendPing(void);

		/// \details Permet de configurer l'état d'un pin de l'équipement distant
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetPinDigital(unsigned char pin, unsigned char status);

		/// \details Permet de configurer la valeur d'un pin analogique (0-1023) de l'équipement distant
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetPinAnalog(unsigned char pin, unsigned short val);

		/// \details Permet de configurer la vitesse des moteurs associés au moteur 1
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetMoteur1(char vitesseGauche, char vitesseDroite);

		/// \details Permet de configurer la vitesse des moteurs associés au moteur 2
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetMoteur2(char vitesseGauche, char vitesseDroite);

		/// \details Permet de d'afficher un message sur l'afficheur de l'équipement distant
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendDisplayString(char* message, unsigned char len);

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

		/// \details Permet de configurer l'angle de rotation d'un servoMoteur désigné par son index GPIO (1-8)
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetGpioPwm(
			unsigned char gpio, ///< Index du gpio à gérer
			unsigned char angle ///< Nouvel angle à appliquer
		);

		/// \details Permet de configurer l'état d'un relai
		/// \return  Retourne true si le message a bien été traité et false sinon
		bool sendSetRelay(
			unsigned char relay, ///< Index du relai à positionner (1-8)
			bool          status ///< Nouvel état LOW=ouvert HIGH=Fermé
		);

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
		LibCanProtCommon canProtCom;


		//========== Routines de gestion des messages ========

		/// \details Envoi le message présent dans le buffer
		void envoiMessage(void);

		/// \details Attends une réponse avec un time-out
		/// \return  Retourne true si un message est présent dans le buffer m_bufferCan et false sinon
		bool waitReponse(
			unsigned char cmd  ///< Commande attendue
		);

  protected:
};



/// \class LibCanProtRecv
/// \brief La librairie LibCanProtRecv permet de gérer le protocole utilisé entre le poste de commande du robot et le robot
/// \brief Elle s'appuie sur la librairie \ref LibCan2515

#include "LibMoteur.h"
#include "LibColor.h"
#include "LibGpio.h"

#define CAN_BUS_MAX_RELAY 8

class LibCanProtRecv
{
public:
	/// \details 
	LibCanProtRecv(void);

	/// \details Permet de donner les références du driver du bus CAN
	void setCanBusDriver(
		LibCan2515* p_canBus ///< Pointeur vers le gestionnaire du bus CAN
	) {
		mp_canBus = p_canBus;
	}

	/// \details Permet de donner les références des moteurs
	/// \details Il est possible de gérer 2 moteurs
	void setMoteurDriver(
		unsigned char moteur, ///< Moteur à configurer (0-1)
		LibMoteur* p_moteur   ///< Pointeur vers le gestionnaire de moteur
	);

	/// \details Permet de donner les références du driver du détecteur de couleur
	void setColorDriver(
		LibTcs3472* p_color ///< Pointeur vers le gestionnaire du détecteur de couleur
	) {
		mp_color = p_color;
	}

	/// \details Permet de donner les références du driver du détecteur de couleur
	void setColorDriver(
		LibMultiTcs3472* p_color ///< Pointeur vers le gestionnaire du détecteur de couleur
	) {
		mp_color_multi = p_color;
	}

	/// \details Permet de donner les références du driver du gestionnaire de GPIO
	void setGpioDriver(
		LibGpio* p_gpio ///< Pointeur vers le gestionnaire de GPIO
	) {
		mp_gpio = p_gpio;
	}

	void setPinRelayList(unsigned char nbRelay, unsigned char* pinRelay);

	/// \details Permet de valider l'interface CAN et le gestion du protocole
	/// \return Retourne true si le bus CAN est prêt et false sinon 
	bool begin(void);

	/// \details Permet de traiter l'arrivée d'un éventuel message
	/// \return Retourne true si un message a été traité et false sinon
	bool gestionMessage(
		// unsigned char csPin ///< Pin associée au signal CS
	);

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
	unsigned char m_nbRelay=0;
	unsigned char m_pinRelay[CAN_BUS_MAX_RELAY];

	LibCan2515* mp_canBus = NULL;
	LibMoteur* mp_moteur1 = NULL;
	LibMoteur* mp_moteur2 = NULL;
	LibTcs3472*      mp_color = NULL;
	LibMultiTcs3472* mp_color_multi = NULL;
	LibGpio* mp_gpio = NULL;

	LibCanProtCommon canProtCom;

	void ping(void);
	void setPinDigital(void);
	void getPinDigital(void);
	void setPinAnalog(void);
	void getPinAnalog(void);
	void setMoteur(unsigned char cmd);
	void displayString(void);
	void getColor(void);
	void setGpioPwm(void);
	void setRelay(void);

	void returnAck(unsigned char cmd);
	void returnGetPinDigital(unsigned char pin, unsigned char status);
	void returnGetPinAnalog(unsigned char pin, unsigned char value);
	void returnGetColor(unsigned char color);

	//========== Routines de gestion des messages ========

	/// \details Envoi le message présent dans le buffer
	void envoiMessage(void);

protected:
};


#endif