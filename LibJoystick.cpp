/*!
 * \file 	LibJoystick.cpp
 * \brief 	Le fichier source de la classe LibJoystick
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibJoystick.h"


LibJoystick::LibJoystick( unsigned char potardX, unsigned char potardY ) {
  m_potardX = potardX;
  m_potardY = potardY;
  pinMode(m_potardX, INPUT);
  pinMode(m_potardY, INPUT);
}

void LibJoystick::calculateVitesse(void) {

/// \todo

}


char LibJoystick::getVitesseGauche(void) { return m_vitesseGauche; }


char LibJoystick::getVitesseDroite(void) { return m_vitesseDroite; }


void LibJoystick::getVitesses( char * vitesseGauche, char * vitesseDroite ) {
  calculateVitesse();
  *vitesseGauche = m_vitesseGauche;
  *vitesseDroite = m_vitesseDroite;
}

