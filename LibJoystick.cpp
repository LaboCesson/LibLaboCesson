/*!
 * \file 	LibJoystick.cpp
 * \brief 	Le fichier source de la classe LibJoystick
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibJoystick.h"


LibJoystick::LibJoystick(
  unsigned char potardX,
  unsigned char potardY, 
  unsigned char  boutonBoost,
  unsigned char  boutonRotationGauche,
  unsigned char  boutonRotationDroite ) 
{
  m_potardX = potardX;
  m_potardY = potardY;
  m_boutonBoost = boutonBoost;
  m_boutonRotationGauche = boutonRotationGauche;
  m_boutonRotationDroite = boutonRotationDroite;
  pinMode(m_potardX, INPUT);
  pinMode(m_potardY, INPUT);
  pinMode(m_boutonBoost, INPUT_PULLUP);
  pinMode(m_boutonRotationGauche, INPUT_PULLUP);
  pinMode(m_boutonRotationDroite, INPUT_PULLUP);
  m_debug = false;
}

void LibJoystick::calculateVitesse(void) {
  char joystickX = analogRead(m_potardX);
  char joystickY = analogRead(m_potardY);
  bool boost = (digitalRead(m_boutonBoost) == LOW ? true : false);
  bool rotationGauche = (digitalRead(m_boutonRotationGauche) == LOW ? true : false);
  bool rotationDroite = (digitalRead(m_boutonRotationDroite) == LOW ? true : false);

  if (m_debug) {
    Serial.print("JOYSTICK_X="); Serial.print(joystickX); Serial.print(",");
    Serial.print("JOYSTICK_Y="); Serial.print(joystickY); Serial.print(",");
    if (boost) Serial.print(" BOOST ");
    if (rotationGauche) Serial.print(" ROTATION GAUCHE ");
    if (rotationDroite) Serial.print(" ROTATION DROITE ");
    Serial.println("");
  }


/// \todo

}


char LibJoystick::getVitesseGauche(void) { return m_vitesseGauche; }


char LibJoystick::getVitesseDroite(void) { return m_vitesseDroite; }


void LibJoystick::getVitesses( char * vitesseGauche, char * vitesseDroite ) {
  calculateVitesse();
  *vitesseGauche = m_vitesseGauche;
  *vitesseDroite = m_vitesseDroite;
}


void LibJoystick::setDebug(bool debug) { m_debug = debug; }
