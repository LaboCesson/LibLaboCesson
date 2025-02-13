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
    Serial.print("JOYSTICK_X = "); Serial.print((int)joystickX); Serial.print("  ");
    Serial.print("JOYSTICK_Y = "); Serial.print((int)joystickY); Serial.print("  ");
    if (boost) Serial.print(" BOOST ");
    if (rotationGauche) Serial.print(" ROTATION GAUCHE ");
    if (rotationDroite) Serial.print(" ROTATION DROITE ");
    Serial.println("");
  }

  calcul(joystickX, joystickY, boost, rotationGauche, rotationDroite);
/// \todo

}

void LibJoystick::calcul(unsigned short valX, unsigned short valY, bool boost, bool rotationGauche, bool rotationDroite) {
  int Vmin = 0;
  int Vmax = (boost == true ? 100 : 50);

  if (rotationGauche == true) {
    m_vitesseGauche = -Vmax;
    m_vitesseDroite = Vmax;
    return;
  }
 
  if (rotationDroite == true) {
     m_vitesseGauche = Vmax;
     m_vitesseDroite = -Vmax;
     return;
   }

 
  if (valY < m_minY) {
    m_vitesseDroite = map(valY, m_minY, 0, Vmin, Vmax);
    m_vitesseGauche = map(valY, m_minY, 0, Vmin, Vmax);
  }
  else if (valY > m_maxY) {
    m_vitesseDroite = map(valY, m_maxY, 1023, Vmin, Vmax);
    m_vitesseGauche = map(valY, m_maxY, 1023, Vmin, Vmax);
  }
  else {
    m_vitesseDroite = 0;
    m_vitesseGauche = 0;
  }

  if (valX < m_minX) {

    int xMapped = map(valX, m_minX, 0, Vmin, Vmax);

    m_vitesseDroite = m_vitesseDroite - xMapped;
    m_vitesseGauche = m_vitesseGauche + xMapped;

    if (m_vitesseDroite < Vmin) {
      m_vitesseDroite = Vmin;
    }

    if (m_vitesseGauche > Vmax) {
      m_vitesseGauche = Vmax;
    }
  }

  if (valX > m_maxX) {

    int xMapped = map(valX, m_maxX, 1023, Vmin, Vmax);

    m_vitesseDroite = m_vitesseDroite + xMapped;
    m_vitesseGauche = m_vitesseGauche - xMapped;

    if (m_vitesseDroite > Vmax) {
      m_vitesseDroite = Vmax;
    }

    if (m_vitesseGauche < Vmin) {
      m_vitesseGauche = Vmin;
    }

  }

  if (m_vitesseDroite < m_minMotorD) {
    m_vitesseDroite = 0;
  }

  if (m_vitesseGauche < m_minMotorG) {
    m_vitesseGauche = 0;
  }


  if (valY < 512) {
    m_vitesseGauche = -m_vitesseGauche;
    m_vitesseDroite = -m_vitesseDroite;
  }
}


char LibJoystick::getVitesseGauche(void) { return m_vitesseGauche; }


char LibJoystick::getVitesseDroite(void) { return m_vitesseDroite; }


void LibJoystick::getVitesses( char * vitesseGauche, char * vitesseDroite ) {
  calculateVitesse();
  *vitesseGauche = m_vitesseGauche;
  *vitesseDroite = m_vitesseDroite;
}


void LibJoystick::setDebug(bool debug) { m_debug = debug; }
