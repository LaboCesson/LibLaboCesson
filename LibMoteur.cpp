/*!
 * \file 	LibMoteur.cpp
 * \brief 	Le fichier source de la classe LibMoteur
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibMoteur.h"

LibMoteur::LibMoteur(unsigned char pinEna, unsigned char pinIn1, unsigned char pinIn2, unsigned char pinIn3, unsigned char pinIn4, unsigned char pinEnb) {
  pinMode(m_enableGauche  = pinEna, OUTPUT); analogWrite(m_enableGauche, 0);
  pinMode(m_enableDroite  = pinEnb, OUTPUT); analogWrite(m_enableDroite, 0);
  
  pinMode(m_avantGauche   = pinIn1, OUTPUT); digitalWrite(m_avantGauche,  LOW);
  pinMode(m_arriereGauche = pinIn2, OUTPUT); digitalWrite(m_arriereGauche,LOW);
  pinMode(m_arriereDroite = pinIn3, OUTPUT); digitalWrite(m_arriereDroite,LOW);
  pinMode(m_avantDroite   = pinIn4, OUTPUT); digitalWrite(m_avantDroite,  LOW);
}


void LibMoteur::moteurGauche(char vitesse) {
  unsigned char d = getDirection(vitesse);

  if( d != m_directionGauche ) {
    m_directionGauche = d;
    digitalWrite(m_avantGauche,  LOW);
    digitalWrite(m_arriereGauche,LOW);
    if( m_directionGauche == DIRECTION_AVANT  ) digitalWrite(m_avantGauche,  HIGH);
    if( m_directionGauche == DIRECTION_ARRIERE) digitalWrite(m_arriereGauche,HIGH);
  }
  analogWrite(m_enableGauche, getVitesse(vitesse));
}


void LibMoteur::moteurDroit(char vitesse) {
  unsigned char d = getDirection(vitesse);

  if( d != m_directionDroite ) {
    m_directionDroite = d;
    digitalWrite(m_avantDroite,  LOW);
    digitalWrite(m_arriereDroite,LOW);
    if( m_directionDroite == DIRECTION_AVANT  ) digitalWrite(m_avantDroite,  HIGH);
    if( m_directionDroite == DIRECTION_ARRIERE) digitalWrite(m_arriereDroite,HIGH);
  }
  analogWrite(m_enableDroite, getVitesse(vitesse));
}


void LibMoteur::moteurs(char vitesse) {
  moteurGauche(vitesse);
  moteurDroit(vitesse);
}


unsigned char LibMoteur::getDirection(char vitesse) {
  if( vitesse == 0 ) return DIRECTION_AUCUNE;
  return( vitesse > 0 ? DIRECTION_AVANT : DIRECTION_ARRIERE);
}


unsigned char LibMoteur::getVitesse(char vitesse) {
  unsigned char v = ( vitesse > 0 ? vitesse : -vitesse);
  return map(v, 0, 100, 0, 255);
}


