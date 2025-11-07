/*!
 * \file 	LibClavier.cpp
 * \brief 	Le fichier source de la classe LibClavier
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibClavier.h"


LibClavier::LibClavier( unsigned char nbTouche, char * p_touche,
                        unsigned char nbPinOut, unsigned char * p_pinOut,
                        unsigned char nbPinIn,  unsigned char * p_pinIn,
                        LibMcp23008 * p_pinExt ) {
  m_nbTouche = nbTouche;
  mp_touche  = p_touche;
  mp_matrice = new LibMatrice(nbPinOut, p_pinOut, nbPinIn,  p_pinIn, p_pinExt);
}


void LibClavier::begin(void) {
  mp_matrice->begin();
}


char LibClavier::getTouche(void) {
  unsigned char index = mp_matrice->getIndex();
  if( (index == 0) || (index > m_nbTouche) ) return 0;
  return mp_touche[index-1];
}

