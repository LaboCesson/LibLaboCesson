/*!
 * \file 	LibJoystick.cpp
 * \brief 	Le fichier source de la classe LibJoystick
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibJoystick.h"


LibJoystick::LibJoystick(
  unsigned short x0,        unsigned short y0,
  unsigned short deltaX,    unsigned short deltaY,
  unsigned short vNormale,  unsigned short vBoost,
  unsigned short minMotorG, unsigned short minMotorD ) {

  m_minX = x0 - (deltaX / 2);
  m_maxX = x0 + (deltaX / 2);
  m_minY = y0 - (deltaY / 2);
  m_maxY = y0 + (deltaY / 2);

  m_vNormale = vNormale;
  m_vBoost   = vBoost;

  m_minMotorG = minMotorG;
  m_minMotorD = minMotorD;
}


void LibJoystick::calculVitesses(unsigned short valX, unsigned short valY, bool boost, bool rotationGauche, bool rotationDroite) {
  char Vmin = 0;
  char Vmax = (boost == true ? m_vBoost : m_vNormale);

  if (rotationGauche == true) {
    m_vitesseGauche = -Vmax;
    m_vitesseDroite = Vmax;
    if( m_debug ) trace(valX, valY, boost, rotationGauche, rotationDroite);
    return;
  }
 
  if (rotationDroite == true) {
     m_vitesseGauche = Vmax;
     m_vitesseDroite = -Vmax;
    if( m_debug ) trace(valX, valY, boost, rotationGauche, rotationDroite);
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

  if( m_debug ) trace(valX, valY, boost, rotationGauche, rotationDroite);
}


void LibJoystick::getVitesses( char * vitesseGauche, char * vitesseDroite ) {
  *vitesseGauche = m_vitesseGauche;
  *vitesseDroite = m_vitesseDroite;
}


void LibJoystick::setDebug(bool debug, unsigned short periodeTrace ) { m_debug = debug; m_periodeTrace = periodeTrace; }


void LibJoystick::trace(unsigned short valX, unsigned short valY, bool boost, bool rotationGauche, bool rotationDroite) {
  static unsigned long nextTrace = millis();

  if( nextTrace > millis() ) return;
  nextTrace += m_periodeTrace;

  Serial.print(F("X=")); Serial.print(valX); Serial.print(" ");
  Serial.print(F("Y=")); Serial.print(valY); Serial.print(" ");
  Serial.print(boost ? "B" :"");
  Serial.print(rotationGauche ? "G" :"");
  Serial.print(rotationDroite ? "D" :""); Serial.print(" ");
  Serial.print(F("=>"));
  Serial.print(F("Gauche=")); Serial.print((int)m_vitesseGauche); Serial.print(" ");
  Serial.print(F("Droite=")); Serial.print((int)m_vitesseDroite); Serial.println("");
}
