/*!
 * \file 	LibMoteurOmni.cpp
 * \brief 	Le fichier source de la classe LibMoteurOmni
 * \author  LaboCesson
 * \version 1.0
 * \date    Novembre 2025
 */

#include "LibMoteurOmni.h"

#define PAMI_MOTEURS1_IN1 5
#define PAMI_MOTEURS1_IN2 6
#define PAMI_MOTEURS1_IN3 7
#define PAMI_MOTEURS1_IN4 8

#define PAMI_MOTEURS2_IN1 39
#define PAMI_MOTEURS2_IN2 38
#define PAMI_MOTEURS2_IN3 37
#define PAMI_MOTEURS2_IN4 36

// int omniVitesse[8][4] = {
// //  avantGauche, avantDroit, arriereGauche, arriereDroit
//   {  1, 1, 1, 1}, // 0
//   {  1, 0, 0, 1}, // 45
//   {  1,-1, 1,-1}, // 90
//   {  0,-1,-1, 0}, // 135
//   { -1,-1,-1,-1}, // 180
//   { -1, 0, 0,-1}, // 225
//   { -1, 1,-1, 1}, // 270
//   {  0, 1, 1, 0}  // 315  
// };


LibMoteurOmni::LibMoteurOmni( void ) :
  moteur1(PAMI_MOTEURS1_IN1, PAMI_MOTEURS1_IN2, PAMI_MOTEURS1_IN3, PAMI_MOTEURS1_IN4),
  moteur2(PAMI_MOTEURS2_IN1, PAMI_MOTEURS2_IN2, PAMI_MOTEURS2_IN3, PAMI_MOTEURS2_IN4)
{
  m_debug = false;
  moteur1.setDirection(true,false);
  moteur2.setDirection(true,false);
}

void LibMoteurOmni::avantGauche  ( int vitesse ) { moteur2.moteurGauche(vitesse); }
void LibMoteurOmni::avantDroit   ( int vitesse ) { moteur2.moteurDroit (vitesse); }
void LibMoteurOmni::arriereGauche( int vitesse ) { moteur1.moteurDroit (vitesse); }
void LibMoteurOmni::arriereDroit ( int vitesse ) { moteur1.moteurGauche(vitesse); }


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
