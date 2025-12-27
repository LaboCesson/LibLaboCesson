/*!
 * \file 	LibMoteurOmni.cpp
 * \brief 	Le fichier source de la classe LibMoteurOmni
 * \author  LaboCesson
 * \version 1.0
 * \date    Novembre 2025
 */

#include "LibMoteurOmni.h"


LibMoteurOmni::LibMoteurOmni(
  unsigned char pinAvantGaucheAvance,
  unsigned char pinAvantGaucheRecule,
  unsigned char pinAvantDroitAvance,
  unsigned char pinAvantDroitRecule,
  unsigned char pinArriereGaucheAvance,
  unsigned char pinArriereGaucheRecule,
  unsigned char pinArriereDroitAvance,
  unsigned char pinArriereDroitRecule
) :
  moteurAvant  (pinAvantGaucheAvance,   pinAvantGaucheRecule,   pinAvantDroitAvance,   pinAvantDroitRecule),
  moteurArriere(pinArriereGaucheAvance, pinArriereGaucheRecule, pinArriereDroitAvance, pinArriereDroitRecule)
{
  m_debug = false;
  moteurAvant.setDirection(true, false);
  moteurArriere.setDirection(true, false);
}


void LibMoteurOmni::avantGauche  ( int vitesse ) { moteurAvant.moteurGauche(vitesse); }
void LibMoteurOmni::avantDroit   ( int vitesse ) { moteurAvant.moteurDroit (vitesse); }
void LibMoteurOmni::arriereGauche( int vitesse ) { moteurArriere.moteurDroit (vitesse); }
void LibMoteurOmni::arriereDroit ( int vitesse ) { moteurArriere.moteurGauche(vitesse); }


int omniAvance[11][4] = {
//  avantGauche, avantDroit, arriereGauche, arriereDroit
  {  0, 0, 0, 0}, // 0
  {  1, 1, 1, 1}, // 0
  {  1, 0, 0, 1}, // 45
  {  1,-1,-1, 1}, // 90
  {  0,-1,-1, 0}, // 135
  { -1,-1,-1,-1}, // 180
  { -1, 0, 0,-1}, // 225
  { -1, 1, 1,-1}, // 270
  {  0, 1, 1, 0}, // 315  
  { -1, 1,-1, 1}, // Gauche
  {  1,-1, 1,-1}  // Droite  
};


void LibMoteurOmni::avance( t_omni_direction direction, int vitesse ) {
  avantGauche  (vitesse*omniAvance[direction][0]);
  avantDroit   (vitesse*omniAvance[direction][1]);
  arriereGauche(vitesse*omniAvance[direction][2]);
  arriereDroit (vitesse*omniAvance[direction][3]);
}


void LibMoteurOmni::stop( void ) {
  this->avance(STOP,0);
}


void LibMoteurOmni::setDebug(bool debug) { m_debug = debug; }
