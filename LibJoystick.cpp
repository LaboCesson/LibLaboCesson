/*!
 * \file 	LibJoystick.cpp
 * \brief 	Le fichier source de la classe LibJoystick
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibJoystick.h"

LibJoystick::LibJoystick();


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


void LibJoystick::begin(
  unsigned short x0,        unsigned short y0,
  unsigned short deltaX,    unsigned short deltaY,
  unsigned short vNormale,  unsigned short vBoost,
  unsigned short minMotorG, unsigned short minMotorD) {

  m_minX = x0 - (deltaX / 2);
  m_maxX = x0 + (deltaX / 2);
  m_minY = y0 - (deltaY / 2);
  m_maxY = y0 + (deltaY / 2);

  m_vNormale = vNormale;
  m_vBoost = vBoost;

  m_minMotorG = minMotorG;
  m_minMotorD = minMotorD;
}


void LibJoystick::calculVitesses(unsigned short xAxis, unsigned short yAxis, bool boost, bool rotationGauche, bool rotationDroite) {
  char VMax = (boost == true ? m_vBoost : m_vNormale);

  int motorSpeedD = 0;
  int motorSpeedG = 0;

  int YD = map(yAxis, 0, 1023, -VMax, VMax);
  int XD = map(xAxis, 0, 1023, VMax, -VMax);

  int YG = map(yAxis, 0, 1023, -VMax, VMax);
  int XG = map(xAxis, 0, 1023, -VMax, VMax);


  if (YD >= 0 && XD >= 0) {
    motorSpeedD = XD;
    if (YD > XD) {
      motorSpeedD = YD;
    }
  }

  if (YD < 0 && XD < 0) {
    motorSpeedD = XD;
    if (YD < XD) {
      motorSpeedD = YD;
    }
  }

  if (YD >= 0 && XD < 0) {
    motorSpeedD = YD + XD;
  }

  if (YD < 0 && XD >= 0) {
    motorSpeedD = XD + YD;
  }

  if (YG >= 0 && XG >= 0) {
    motorSpeedG = XG;
    if (YG > XG) {
      motorSpeedG = YG;
    }
  }

  if (YG < 0 && XG < 0) {
    motorSpeedG = XG;
    if (YG < XG) {
      motorSpeedG = YG;
    }
  }

  if (YG >= 0 && XG < 0) {
    motorSpeedG = YG + XG;
  }

  if (YG < 0 && XG >= 0) {
    motorSpeedG = XG + YG;
  }


  if (abs(motorSpeedD) < m_minMotorD) {
    motorSpeedD = 0;
  }
  if (abs(motorSpeedG) < m_minMotorG) {
    motorSpeedG = 0;
  }

  m_vitesseGauche = motorSpeedG;
  m_vitesseDroite = motorSpeedD;

  if (m_debug) trace(xAxis, yAxis, boost, rotationGauche, rotationDroite);

  return;
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
