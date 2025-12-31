/*!
 * \file 	LibCan2515.cpp
 * \brief 	Le fichier source de la classe LibCan2515
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#include "LibCan.h"
#include "SPI.h"


//=====================================
// Class LibCan2515
//=====================================

LibCan2515::LibCan2515(unsigned char csPin) : can(csPin) {
  m_csPin = csPin;
  this->init();
}


LibCan2515::LibCan2515(unsigned char sckPin, unsigned char misoPin, unsigned char mosiPin, unsigned char csPin) : can(csPin) {
  m_sckPin  = sckPin;
  m_misoPin = misoPin;
  m_mosiPin = mosiPin;
  m_csPin   = csPin;
  SPI.begin( m_sckPin, m_misoPin, m_mosiPin, m_csPin );
  this->init();
}


void LibCan2515::init(void) {
  m_debug = false;
}


bool LibCan2515::begin(	unsigned char canId, unsigned int speedSet, unsigned char clockSet) {
  unsigned char count = 0;

  m_canId    = canId;
  m_speedSet = speedSet;
  m_clockSet = clockSet;

  while ((can.begin(m_speedSet,m_clockSet) != CAN_OK) && (count++ < 20)) {
    Serial.println("CAN init fail, retry...");
    delay(200);
  }

  if( count < 20 ) {
    Serial.println("CAN init OK!");
    return true;
  }
  else { 
    Serial.println("CAN init HS!");
    return false;
  }
}


unsigned char LibCan2515::getMessage( unsigned char * p_buf) {
  unsigned char len;

  if (CAN_MSGAVAIL != can.checkReceive()) return 0;

  can.readMsgBuf(&len, p_buf);
  displayMessage(p_buf, len);
  return len;
}


void LibCan2515::sendMessage(unsigned char * buf, unsigned char len) {
  can.sendMsgBuf(m_canId,0,len,buf);
  displayMessage(buf,len);
}


void LibCan2515::displayMessage( unsigned char * p_buf, unsigned char len ) {
  if( m_debug == false ) return;
  Serial.print("->");
  Serial.print(can.getCanId(), HEX);
  Serial.print(" : ");
  for(int i=0;i<len;i++){
    Serial.print(p_buf[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
}


void LibCan2515::setDebug(bool debug) { m_debug = debug; }



//=====================================
// Class LibCanProt
//=====================================

#include "LibCanProtDef.h"


LibCanProt::LibCanProt() {
  m_debug = false;
  m_begin = false;
}


bool LibCanProt::begin(void) {
  if( mp_canBus->begin() == false ) return false;
  m_begin = true;
  return true;
}




bool LibCanProt::gestionMessage(void) {
  unsigned char len = mp_canBus->getMessage(m_bufferCan);

  if( len == 0 ) return false;

  unsigned char cmd = m_bufferCan[0]>>4;

  displayMessageRecu(cmd);
  switch(cmd) {
    case BUS_CAN_SET_PIN_DIGITAL    : setPinDigital(); break;
    case BUS_CAN_GET_PIN_DIGITAL    : break;
    case BUS_CAN_SET_PIN_ANALOGIQUE : setPinAnalog();  break;
    case BUS_CAN_GET_PIN_ANALOGIQUE : break;
    case BUS_CAN_SET_MOTEUR         : setMoteur();     break;
    case BUS_CAN_DISPLAY_STRING     : displayString(); break;
    case BUS_CAN_CUSTOM_COMMAND     : break;
  }
  return true;
}


void LibCanProt::setPinDigital(void) {
  unsigned char pin    = m_bufferCan[1];
  unsigned char status = m_bufferCan[2];
  digitalWrite(pin,status);
  if( m_debug == true ) {
    Serial.print(pin); Serial.print("="); Serial.print(status); Serial.println("");
  }
}


void LibCanProt::setPinAnalog (void) {
  unsigned char  pin = m_bufferCan[1];
  unsigned short val = m_bufferCan[2]&0x03;
  val <<= 8;
  val += m_bufferCan[3];
  analogWrite(pin,val);
  if( m_debug == true ) {
    Serial.print(pin); Serial.print("="); Serial.print(val); Serial.println("");
  }
}


void LibCanProt::setMoteur(void) {
  // char moteur        = p_buf[1];
  // char vitesseGauche = p_buf[2];
  // char vitesseDroite = p_buf[3];
  // moteur.moteurGauche(vitesseGauche);
  // moteur.moteurDroit(vitesseDroite);
}


void LibCanProt::displayString(void) {
  // afficheur.displayBuffer(&(p_buf[2]), p_buf[1]);
}



void LibCanProt::sendSetPinDigital( unsigned char pin, unsigned char status ) {
  unsigned char * p_buf = m_bufferCan;
  *p_buf++ = (BUS_CAN_SET_PIN_DIGITAL<<4)+2;
  *p_buf++ = pin;
  *p_buf++ = status;
  mp_canBus->sendMessage(m_bufferCan, p_buf-m_bufferCan);
}


void LibCanProt::sendSetPinAnalog( unsigned char pin, unsigned char val ) {

}


void LibCanProt::sendSetMoteur( unsigned char moteur, char vitesseGauche, char vitesseDroite ) {

}


void LibCanProt::sendDisplayString( char * message, unsigned char len) {

}




void LibCanProt::displayMessageRecu( unsigned char cmd ) {
  if( m_debug == false ) return;
  Serial.print("CAN recv=> ");
  displayMessageString(cmd);
}


void LibCanProt::displayMessageEnvoye( unsigned char cmd ) {
  if( m_debug == false ) return;
  Serial.print("CAN send=> ");
  displayMessageString(cmd);
}


void LibCanProt::displayMessageString( unsigned char cmd ) {
  switch(cmd) {
    case BUS_CAN_SET_PIN_DIGITAL    : Serial.print("SET_PIN_DIGITAL ");    break;
    case BUS_CAN_GET_PIN_DIGITAL    : Serial.print("GET_PIN_DIGITAL ");    break;
    case BUS_CAN_SET_PIN_ANALOGIQUE : Serial.print("SET_PIN_ANALOGIQUE "); break;
    case BUS_CAN_GET_PIN_ANALOGIQUE : Serial.print("GET_PIN_ANALOGIQUE "); break;
    case BUS_CAN_SET_MOTEUR         : Serial.print("SET_MOTEUR ");         break;
    case BUS_CAN_DISPLAY_STRING     : Serial.print("DISPLAY_STRING ");     break;
    case BUS_CAN_CUSTOM_COMMAND     : Serial.print("CUSTOM_COMMAND ");     break;
    default :                         Serial.print("!! INCONNUE !!");     break;
  }
}


void LibCanProt::setDebug(bool debug) { m_debug = debug; }

