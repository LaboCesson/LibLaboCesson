/*!
 * \file 	LibJoystick.h
 * \brief 	Le fichier de définition de la classe LibJoystick
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_JOYSTICK_H_
#define _LIB_JOYSTICK_H_

#include "arduino.h"


/// \class LibJoystick
/// \brief Gestion d'un Joystick
/// \details Permet d'utiliser un Joystick pour piloter un robot équipé d'un moteur gauche et d'un moteur droit
class LibJoystick
{
  public:
    LibJoystick(
      unsigned char  potardX,              ///< Entrée analogique associée au potentiometre d'axe X
      unsigned char  potardY,              ///< Entrée analogique associée au potentiometre d'axe Y
      unsigned char  boutonBoost,          ///< Numéro du pin associé au bouton Boost
      unsigned char  boutonRotationGauche, ///< Numéro du pin associé au bouton Rotation Gauche
      unsigned char  boutonRotationDroite  ///< Numéro du pin associé au bouton Rotation Droite
    );

    /// \return Permet de calculer les vitesses à appliquer
    /// \details Cette fonction est à appeler avant d'appeller les fonctions getVitesseDroite et getVitesseGauche
    void calculateVitesse(void);

    /// \return Retourne la vitesse à appliquer au moteur gauche
    char getVitesseGauche(void);

    /// \return Retourne la vitesse à appliquer au moteur droit
    char getVitesseDroite(void);
	
    /// \return Retourne la vitesse à appliquer au moteur gauche
    void getVitesses( char * vitesseGauche, char * vitesseDroite );

    /// \details Permet de valider l'affichage de message de debug
    void setDebug(
      bool debug ///< si true les messages de debug sont affichés
    );

  private:
    bool m_debug;
    unsigned char m_potardX;
    unsigned char m_potardY;
    unsigned char m_boutonBoost;
    unsigned char m_boutonRotationGauche;
    unsigned char m_boutonRotationDroite;
    char m_vitesseGauche;
    char m_vitesseDroite;

  protected:
};

#endif