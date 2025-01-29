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
      unsigned char   potardX, ///< Entrée analogique associé au potentiometre d'axe X
      unsigned char   potardY  ///< Entrée analogique associé au potentiometre d'axe Y
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

  private:
    unsigned char * m_potardX;
    unsigned char * m_potardY;
    char m_vitesseGauche;
    char m_vitesseDroite;

  protected:
};

#endif