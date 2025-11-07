/*!
 * \file 	LibGpio.cpp
 * \brief 	Le fichier source de la classe LibGpio
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibGpio.h"


LibGpio::LibGpio(unsigned char * pinList, unsigned char nbGpioInTab) {
  m_nbGpio = nbGpioInTab;
  if( m_nbGpio > 16 ) m_nbGpio = 16; // Par sécurité

  m_gpio = (t_gpio*)malloc(sizeof(t_gpio) * m_nbGpio);

  for(unsigned char i=0; i<m_nbGpio; i++ ) {
    m_gpio[i].pin   = pinList[i];
    m_gpio[i].type  = PAMI_GPIO_UNUSED;
    m_gpio[i].value = 0;
  }
  m_debug = false;
}


bool LibGpio::configure(unsigned char gpioIdx, t_gpioMode gpioType, unsigned short defaultValue) {
  if( gpioIdx >= m_nbGpio) return LIB_GPIO_ERROR;

  if( m_gpio[gpioIdx].type != PAMI_GPIO_UNUSED ) { trace( gpioIdx, "est déja défini" ); return LIB_GPIO_ERROR; }

  switch(gpioType) {
    case PAMI_GPIO_UNUSED :
        // Rien à faire
        trace( gpioIdx, "set as unused" );
        break;
    case PAMI_GPIO_INPUT : 
        pinMode(m_gpio[gpioIdx].pin, INPUT_PULLUP);
        m_gpio[gpioIdx].type = PAMI_GPIO_INPUT;
        m_gpio[gpioIdx].value = digitalRead(m_gpio[gpioIdx].pin);
        trace( gpioIdx, "set as an input" );
        break;
    case PAMI_GPIO_OUTPUT :
        m_gpio[gpioIdx].type = PAMI_GPIO_INPUT;
        pinMode(m_gpio[gpioIdx].pin, OUTPUT);
        m_gpio[gpioIdx].type  = PAMI_GPIO_OUTPUT;
        digitalWrite(m_gpio[gpioIdx].pin, m_gpio[gpioIdx].value = defaultValue);
        trace( gpioIdx, " set as an output" );
        break;
    case PAMI_GPIO_PWM :
		    m_gpio[gpioIdx].servo = new Servo();
        m_gpio[gpioIdx].servo->attach(m_gpio[gpioIdx].pin);
        m_gpio[gpioIdx].servo->write( m_gpio[gpioIdx].value = defaultValue);
        m_gpio[gpioIdx].type  = PAMI_GPIO_PWM;
        trace( gpioIdx, " set as a PWM" );
        break;
    default : return LIB_GPIO_ERROR;
  }

  return true;
}


bool LibGpio::set(unsigned char gpioIdx, unsigned short value) {

  if( gpioIdx >= m_nbGpio) return LIB_GPIO_ERROR;

  if (m_debug) {
    Serial.print(F("GPIO: id=")); Serial.print(gpioIdx+1);
    Serial.print(F(",pin=")); Serial.print(m_gpio[gpioIdx].pin);
    Serial.print(F(",val=")); Serial.println(value);
  }

  switch(m_gpio[gpioIdx].type) {
    case PAMI_GPIO_UNUSED : trace( gpioIdx, "try to set an unused pin" ); return false;
    case PAMI_GPIO_INPUT  : trace( gpioIdx, "try to set an input pin" );  return false;
    case PAMI_GPIO_OUTPUT : digitalWrite(m_gpio[gpioIdx].pin, value); break;
    case PAMI_GPIO_PWM    : m_gpio[gpioIdx].servo->write(value); break;
  }

  m_gpio[gpioIdx].value = value;
  return true;
}


unsigned short LibGpio::get(unsigned char gpioIdx) {
  if( gpioIdx >= m_nbGpio) return LIB_GPIO_ERROR;

  switch(m_gpio[gpioIdx].type) {
    case PAMI_GPIO_UNUSED : trace( gpioIdx, "try to read an unused pin" ); return 0;
    case PAMI_GPIO_INPUT  : m_gpio[gpioIdx].value = digitalRead(m_gpio[gpioIdx].pin); break;
    case PAMI_GPIO_OUTPUT : break; // Retourne la valeur en mémoire
    case PAMI_GPIO_PWM    : break; // Retourne la valeur en mémoire
  }

  return m_gpio[gpioIdx].value;
}


void LibGpio::trace(unsigned char gpioIdx, char * msg ) {
  Serial.print(F("LibGpio : "));
  Serial.print(gpioIdx+1);
  Serial.print(F(" : "));
  Serial.println(msg);
}

void LibGpio::setDebug(bool debug) { m_debug = debug; }
