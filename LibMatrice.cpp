/*!
 * \file 	LibMatrice.cpp
 * \brief 	Le fichier source de la classe LibMatrice
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibMatrice.h"

LibMatrice::LibMatrice(unsigned char nbPinOut, unsigned char * p_pinOut,
                       unsigned char nbPinIn, unsigned char * p_pinIn, 
                       LibMcp23008 * p_pinExt=0) {
  m_nbPinOut = nbPinOut;
  mp_pinOut  = p_pinOut;
  m_nbPinIn  = nbPinIn;
  mp_pinIn   = p_pinIn;
  mp_pinExt  = p_pinExt;
  m_debug    = false;
}


void LibMatrice::begin(void) {
  int i;

  for(i=0;i<m_nbPinOut;i++) {
    setPinMode(mp_pinOut[i], INPUT);
    setPinOut(mp_pinOut[i], LOW);
  }

  for(i=0;i<m_nbPinIn;i++) {
    setPinMode(mp_pinIn[i],INPUT_PULLUP);
  }

}


unsigned short LibMatrice::getContacts(void) {
  int i,j;
  unsigned short contact = 0x00;
  unsigned short flag = 0x01;

  for(i=0;i<m_nbPinOut;i++) {
    setPinMode(mp_pinOut[i], OUTPUT);
    setPinOut(mp_pinOut[i], LOW);
    for(j=0;j<m_nbPinIn;j++) {
      if(readPin(mp_pinIn[j]) == LOW) {
        contact |= flag;
      }
      flag <<= 1;
    }
    setPinMode(mp_pinOut[i], INPUT);
  }
  if (m_debug == true) {
    Serial.print("Contacts: ");
    Serial.println(contact, BIN);
  }
  return contact;
}


unsigned char LibMatrice::getIndex(void) {
  int i;
  unsigned char index=0;
  unsigned short contact = getContacts();
  while( contact!=0) {
    index++;
    contact >>= 1;

  }
  return index;
}


unsigned char LibMatrice::getNbContacts(void)  { return (m_nbPinOut*m_nbPinIn); }


void LibMatrice::setPinMode(unsigned char pin, unsigned char mode) {
  if( mp_pinExt != 0 ) {
    mp_pinExt->pinMode(pin,mode);
  } else {
    pinMode(pin,mode);
  }
}


void LibMatrice::setPinOut(unsigned char pin, unsigned char val) {
  if( mp_pinExt != 0 ) {
    mp_pinExt->digitalWrite(pin,val);
  } else {
    digitalWrite(pin,val);
  }
}


unsigned char LibMatrice::readPin(unsigned char pin) {
  if( mp_pinExt != 0 ) {
    return(mp_pinExt->digitalRead(pin));
  } else {
    return(digitalRead(pin));
  }
}

void LibMatrice::setDebug(bool debug) { m_debug = debug; }
