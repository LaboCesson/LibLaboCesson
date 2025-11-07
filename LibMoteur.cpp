/*!
 * \file 	LibMoteur.cpp
 * \brief 	Le fichier source de la classe LibMoteur
 * \author  LaboCesson
 * \version 2.0
 * \date    Octobre 2025
 */

#include "LibMoteur.h"

#define DEFAULT_FREQUENCY 1000
#define DEFAULT_RESOLUTION   8
#define DEFAULT_BASE_CHANNEL 4


#define DEFAULT_SERVO_MIN 1000
#define DEFAULT_SERVO_OFF 1500
#define DEFAULT_SERVO_MAX 2000


LibMoteur::LibMoteur(unsigned char pinIn1, unsigned char pinIn2, unsigned char pinIn3, unsigned char pinIn4 ) {
  commonInit();

  m_typeDriver   = MOTEUR_L298N;

  m_frequency    = DEFAULT_FREQUENCY;
  m_resolution   = DEFAULT_RESOLUTION;
  m_base_channel = DEFAULT_BASE_CHANNEL;

  #ifdef AVR
    pinMode(m_avantGauche   = pinIn1, OUTPUT); analogWrite(m_avantGauche,  0);
    pinMode(m_arriereGauche = pinIn2, OUTPUT); analogWrite(m_arriereGauche,0);
    pinMode(m_arriereDroite = pinIn3, OUTPUT); analogWrite(m_arriereDroite,0);
    pinMode(m_avantDroite   = pinIn4, OUTPUT); analogWrite(m_avantDroite,  0);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    ledcAttachChannel(m_avantGauche   = pinIn1, m_frequency, m_resolution, m_base_channel+0); ledcWrite(m_avantGauche, 0);
    ledcAttachChannel(m_arriereGauche = pinIn2, m_frequency, m_resolution, m_base_channel+1); ledcWrite(m_arriereGauche, 0);
    ledcAttachChannel(m_arriereDroite = pinIn3, m_frequency, m_resolution, m_base_channel+2); ledcWrite(m_arriereDroite, 0);
    ledcAttachChannel(m_avantDroite   = pinIn4, m_frequency, m_resolution, m_base_channel+3); ledcWrite(m_avantDroite, 0);
  #endif
}


LibMoteur::LibMoteur(unsigned char pinGauche, unsigned char pinDroit) {
  commonInit();

  m_typeDriver  = MOTEUR_SERVO;

  mp_servoGauche = new Servo();
  mp_servoGauche->attach(pinGauche, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  mp_servoGauche->writeMicroseconds(DEFAULT_SERVO_OFF);

  mp_servoDroit = new Servo();
  mp_servoDroit->attach(pinDroit, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  mp_servoDroit->writeMicroseconds(DEFAULT_SERVO_OFF);
}


void LibMoteur::commonInit(void) {
  m_lastVitesseGauche = 0;
  m_lastVitesseDroite = 0;
  m_debug = false;
}


void LibMoteur::moteurs(int vitesse) {
  moteurGauche(vitesse);
  moteurDroit(vitesse);
}


void LibMoteur::moteurGauche(int vitesse) {
  vitesse *= m_directionGauche;
  if (vitesse == m_lastVitesseGauche) return;
  m_lastVitesseGauche = vitesse;
  trace("gauche", vitesse);

  switch (m_typeDriver) {
    case MOTEUR_L298N : setVitesseMoteurL298n(vitesse, m_avantGauche, m_arriereGauche); break;
    case MOTEUR_SERVO : setVitesseMoteurServo(vitesse, mp_servoGauche ); break;
    default: break;
  }
}


void LibMoteur::moteurDroit(int vitesse) {
  vitesse *= m_directionDroite;
  if (vitesse == m_lastVitesseDroite) return;
  m_lastVitesseDroite = vitesse;
  trace("droit", vitesse);

  switch (m_typeDriver) {
    case MOTEUR_L298N: setVitesseMoteurL298n(vitesse, m_avantDroite, m_arriereDroite); break;
    case MOTEUR_SERVO: setVitesseMoteurServo(vitesse, mp_servoDroit); break;
    default: break;
  }
}


void LibMoteur::setVitesseMoteurL298n( int vitesse, unsigned char pinAvant, unsigned char pinArriere ) {
  unsigned char vAv = 0; 
  unsigned char vAr = 0;

  if (vitesse > 0) vAv = map( vitesse, 0, 100, 0, 255);
  if (vitesse < 0) vAr = map(-vitesse, 0, 100, 0, 255);

  #ifdef AVR
    analogWrite(pinAvant,  vAv);
    analogWrite(pinArriere,vAr);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    ledcWrite(pinAvant,   vAv);
    ledcWrite(pinArriere, vAr);
  #endif
}


void LibMoteur::setVitesseMoteurServo(int vitesse, Servo* p_servo) {
  unsigned short value = DEFAULT_SERVO_OFF;

  if (vitesse != 0) {
    value = map(vitesse, -100, 100, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  }
 
  p_servo->writeMicroseconds(value);
}


void LibMoteur::setDirection(int dirGauche, int dirDroite) {
  if ((dirGauche != 1) && (dirGauche != -1)) dirGauche = 1;
  if ((dirDroite != 1) && (dirDroite != -1)) dirDroite = 1;

  m_directionGauche = dirGauche;
  m_directionDroite = dirDroite;
}


void LibMoteur::trace(char* side, int vitesse) {
  if (m_debug == false) return;
  Serial.print("Moteur ");
  Serial.print(side);
  Serial.print(":");
  Serial.println((int)vitesse);
}


void LibMoteur::setDebug(bool debug) { m_debug = debug; }
