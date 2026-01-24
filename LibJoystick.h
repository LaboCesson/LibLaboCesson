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
    LibJoystick(void);
    
    LibJoystick(
      unsigned short x0,        ///< Valeur correspondant au point 0 de X
      unsigned short y0,        ///< Valeur correspondant au point 0 de Y
      unsigned short deltaX,    ///< Zone centrale en X ou l'action du Joystick est inopérante
      unsigned short deltaY,    ///< Zone centrale en Y ou l'action du Joystick est inopérante
      unsigned short vNormale,  ///< Valeur de la vitesse normale
      unsigned short vBoost,    ///< Valeur de la vitesse maximum
      unsigned short minMotorG, ///< Vitesse minimum du moteur Gauche
      unsigned short minMotorD  ///< Vitesse minimum du moteur Droite
    );

    void begin(
      unsigned short x0,        ///< Valeur correspondant au point 0 de X
      unsigned short y0,        ///< Valeur correspondant au point 0 de Y
      unsigned short deltaX,    ///< Zone centrale en X ou l'action du Joystick est inopérante
      unsigned short deltaY,    ///< Zone centrale en Y ou l'action du Joystick est inopérante
      unsigned short vNormale,  ///< Valeur de la vitesse normale
      unsigned short vBoost,    ///< Valeur de la vitesse maximum
      unsigned short minMotorG, ///< Vitesse minimum du moteur Gauche
      unsigned short minMotorD  ///< Vitesse minimum du moteur Droite
    );

    // /// \return Permet de calculer les vitesses à appliquer
    // /// \details Cette fonction est à appeler avant d'appeller la fonction getVitesses
    void calculVitesses(unsigned short valX, unsigned short valY, bool boost, bool rotationGauche, bool rotationDroite);

    /// \return Retourne la vitesse à appliquer au moteur gauche
    void getVitesses( char * vitesseGauche, char * vitesseDroite );

    /// \details Permet de valider l'affichage de message de debug
    void setDebug(
      bool debug,                 ///< Si true les messages de debug sont affichés
      unsigned short periodeTrace ///< Période d'affichage des traces en ms
    );

  private:
    void trace(unsigned short valX, unsigned short valY, bool boost, bool rotationGauche, bool rotationDroite);

    bool m_debug;

    char m_vitesseGauche;
    char m_vitesseDroite;

    unsigned short m_vNormale = 0;
    unsigned short m_vBoost   = 0;

    unsigned short m_minX = 512;
    unsigned short m_maxX = 512;
    unsigned short m_minY = 512;
    unsigned short m_maxY = 512;

    unsigned short m_minMotorG = 0; // 60 pour 255, 25 pour 100
    unsigned short m_minMotorD = 0; // 35 pour 255, 15 pour 100

    unsigned short m_periodeTrace = 500; // Période d'affichage des messages de debug en ms

  protected:
};

#endif