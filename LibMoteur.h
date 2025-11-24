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
/// \brief Gestion de moteurs
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
      bool dirGauche, ///< direction du moteur gauche (true=inverse, false= normal (defaut))
      bool dirDroite  ///< direction du moteur droit  (true=inverse, false= normal (defaut))
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
             bool m_directionDroite = true;
             bool m_directionGauche = true;
    unsigned int  m_frequency;    
    unsigned char m_resolution;    

    Servo* mp_servoGauche;
    Servo* mp_servoDroit;

protected:
};


/// \class LibMoteurS
/// \brief Gestion de moteurs avec changement lent de vitesse
/// \details


class LibMoteurS
{
public:
  /// \details Permet de piloter un moteur controllé par un servomoteur 360
  LibMoteurS(
    unsigned char  pinGauche,     ///< Le numéro de la pin connectée au servomteur gauche
    unsigned char  pinDroit,      ///< Le numéro de la pin connectée au servomteur droit
    unsigned short stepTime = 50, ///< Durée en ms d'un pas de changements de vitesse
    unsigned char  stepUp   = 0,  ///< Valeur de la vitesse à augmenter à chaque pas (0=changement immédiat)
    unsigned char  stepDown = 0   ///< Valeur de la vitesse à augmenter à chaque pas (0=changement immédiat)
    );

  /// \details Permet de valider la gestion des moteurs
  void begin(void);

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
    bool dirGauche, ///< direction du moteur gauche (true=normal, false= inverse)
    bool dirDroite  ///< direction du moteur droit (true=normal, false= inverse)
  );

  /// \details Permet de valider l'affichage de message de debug
  void setDebug(
    bool debug ///< si true les messages de debug sont affichés
  );

  /// \details Permet de modifier le sens par défaut des moteurs
  void gestion(void);

private:
  bool m_begin = false;

  LibMoteur    moteur;    ///< Librairie de gestion d'un couple de moteur

  unsigned short m_stepTime;
  unsigned char  m_stepUp, m_stepDown;

  unsigned long  m_nextTimeGestion;

  int m_vitesseGaucheCourante = 0;
  int m_vitesseGaucheCible    = 0;
  int m_vitesseDroiteCourante = 0;
  int m_vitesseDroiteCible    = 0;

  int getNewVitesse(int vitesseCourante, int vitesseCible);
  int getNewVitesseUp(int vitesseCourante, int vitesseCible);
  int getNewVitesseDown(int vitesseCourante, int vitesseCible);


protected:
}; 


#endif