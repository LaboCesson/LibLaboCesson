/*!
 * \file 	LibMoteur.h
 * \brief 	Le fichier de définition de la classe LibMoteur
 * \author  LaboCesson
 * \version 2.0
 * \date    Octobre 2025
 */
//
// Librairie de gestion d'un L298N
//

#ifndef _LIB_MOTEUR_H_
#define _LIB_MOTEUR_H_

#include "arduino.h"

#ifdef AVR#ifdef SERVO_USED
#include "Servo.h"
#endif
#ifdef ARDUINO_ARCH_ESP32
#include "ESP32Servo.h"
#endif


typedef enum {
  MOTEUR_L298N = 0, ///< The motor is controlled with a L298N
  MOTEUR_SERVO = 1, ///< The motor is controlled with a Servo360
} t_driverMoteur;


/// \class LibMoteur
/// \brief Gestion d'un L298N
/// \details
/// <a href="https://www.handsontec.com/dataspecs/L298N%2520Motor%2520Driver.pdf"> L298N Module documentation </a>
class LibMoteur
{
  public:
    /// \details Permet de piloter un moteur controllé par un L298N
    LibMoteur(
		unsigned char pinIn1, ///< Le numéro de la pin connectée à IN1
		unsigned char pinIn2, ///< Le numéro de la pin connectée à IN2
		unsigned char pinIn3, ///< Le numéro de la pin connectée à IN3
		unsigned char pinIn4  ///< Le numéro de la pin connectée à IN4
	  );

    /// \details Permet de piloter un moteur controllé par un servomoteur 360
    LibMoteur(
      unsigned char pinGauche, ///< Le numéro de la pin connectée au servomteur gauche
      unsigned char pinDroit   ///< Le numéro de la pin connectée au servomteur droit
    );

    /// \details Permet de piloter les deux moteurs
    void moteurs(
      int vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
    );

    /// \details Permet de piloter le moteur gauche
    void moteurGauche(
		int vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
	  );

	  /// \details Permet de piloter le moteur droit
    void moteurDroit(
		int vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
	  );

    /// \details Permet de modifier le sens par défaut des moteurs
    void setDirection(
      int dirGauche, ///< direction du moteur gauche (1=normal, -1= inverse)
      int dirDroite  ///< direction du moteur droit (1=normal, -1= inverse)
    );

    /// \details Permet de valider l'affichage de message de debug
    void setDebug(
      bool debug ///< si true les messages de debug sont affichés
    );

private:
    void setVitesseMoteurL298n(
               int  vitesse,    ///< vitesse à appliquer au moteur
      unsigned char pinAvant,   ///< Le numéro de la pin permettant de mettre le moteur dans le sens avant
      unsigned char pinArriere  ///< Le numéro de la pin permettant de mettre le moteur dans le sens arriere
    );

    void setVitesseMoteurServo(
      int    vitesse, ///< vitesse à appliquer au moteur
      Servo* p_servo  ///< Pointeur vers l'objet Servo
    );

    void commonInit(void);
    void trace(char* side, int vitesse);

    t_driverMoteur m_typeDriver;

    bool          m_debug;

    unsigned char m_avantDroite, m_arriereDroite;
    unsigned char m_avantGauche, m_arriereGauche;
             int  m_lastVitesseGauche, m_lastVitesseDroite;
             int  m_directionDroite = 1;
             int  m_directionGauche = 1;
    unsigned int  m_frequency;    
    unsigned char m_resolution;    
    unsigned char m_base_channel;

    Servo* mp_servoGauche;
    Servo* mp_servoDroit;

protected:
};


#endif