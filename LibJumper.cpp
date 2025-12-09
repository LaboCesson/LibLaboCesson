/*!
 * \file 	LibJumper.cpp
 * \brief 	Le fichier source de la classe LibJumper
 * \author  LaboCesson
 * \version 2.0
 * \date    Octobre 2025
 */

#include "LibJumper.h"


LibJumper::LibJumper(unsigned char pinJumperTeam, unsigned char * jumpersPinList, unsigned char   nbPinInTab ) {
  pinMode(m_pinJumperTeam = pinJumperTeam, INPUT_PULLUP);  

  m_nbJumpers = nbPinInTab;
  if (m_nbJumpers > MAX_NB_JUMPERS) m_nbJumpers = MAX_NB_JUMPERS; // Par sécurité
  for (unsigned char i = 0; i < m_nbJumpers; i++) {
    m_jumpers[i] = jumpersPinList[i];
    pinMode(m_jumpers[i],  INPUT_PULLUP);
  }
}


t_pamiTeam LibJumper::getTeam(void) {
  return( ( digitalRead(m_pinJumperTeam) == 0 ? PAMI_TEAM_B : PAMI_TEAM_A ) ) ;
}


unsigned char LibJumper::getPami(void) {
  #ifdef AVR
 
    if( analogRead(m_jumpers[1]) > 100 ) {
      return( ( analogRead(m_jumpers[0]) > 100 ? 0 : 1 ) ) ;
    } else {
      return( ( analogRead(m_jumpers[0]) > 100 ? 2 : 3) ) ;
    }
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    unsigned char value = 0;

    for (unsigned char i = 0; i < m_nbJumpers; i++) {
      value <<= 1;
      if (digitalRead(m_jumpers[i]) == LOW) value |= 1;
    }
    return value;
  #endif
}
