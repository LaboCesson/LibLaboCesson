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
  #ifdef AVR
    SPI.begin();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    SPI.begin( m_sckPin, m_misoPin, m_mosiPin, m_csPin );
  #endif
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


void LibCanProt::setMoteurDriver(unsigned char moteur, LibMoteur* p_moteur) {
  switch (moteur) {
    case 0: mp_moteur1 = p_moteur; break;
    case 1: mp_moteur2 = p_moteur; break;
  }
}



bool LibCanProt::gestionMessage(void) {

  unsigned char len = mp_canBus->getMessage(m_bufferCan);

  if( len == 0 ) return false;

  unsigned char cmd = m_bufferCan[1]>>4;

  if (m_debug == true) {
    Serial.print("CAN recv=> ");
    displayMessageString(cmd);
  }

  switch(cmd) {
    case BUS_CAN_SET_PIN_DIGITAL    : setPinDigital(); break;
    case BUS_CAN_GET_PIN_DIGITAL    : getPinDigital(); break;
    case BUS_CAN_SET_PIN_ANALOGIQUE : setPinAnalog();  break;
    case BUS_CAN_GET_PIN_ANALOGIQUE : getPinAnalog();  break;
    case BUS_CAN_SET_MOTEUR_1       : setMoteur(0);    break;
    case BUS_CAN_SET_MOTEUR_2       : setMoteur(1);    break;
    case BUS_CAN_DISPLAY_STRING     : displayString(); break;
    case BUS_CAN_CUSTOM_COMMAND     : break;
  }
  return true;
}


//========== Routines de traitement des messages reçus  ========


void LibCanProt::setPinDigital(void) {
  unsigned char pin    = m_bufferCan[2];
  unsigned char status = m_bufferCan[3];
  digitalWrite(pin,status);
  displayPinInfo(pin, status);
}


void LibCanProt::setPinAnalog (void) {
  unsigned char  pin = m_bufferCan[2];
  unsigned short val = m_bufferCan[3]&0x03;
  val <<= 8;
  val += m_bufferCan[4];
  analogWrite(pin,val);
  displayPinInfo(pin, val);
}


void LibCanProt::setMoteur(unsigned char moteur) {
  LibMoteur* p_moteur;

  switch (moteur) {
    case 0: p_moteur = mp_moteur1; break;
    case 1: p_moteur = mp_moteur2; break;
    default: return;
  }

  if (p_moteur == NULL) {
    Serial.println("CAN recv=> !!MOTEUR INEXISTANT");
    return;
  }

  char vitesseGauche = m_bufferCan[2];
  char vitesseDroite = m_bufferCan[3];

  p_moteur->moteurGauche(vitesseGauche);
  p_moteur->moteurDroit(vitesseDroite);

  displayVitessesInfo(vitesseGauche, vitesseDroite);
}


void LibCanProt::displayString(void) {
  // afficheur.displayBuffer(&(p_buf[2]), p_buf[1]);
}


void LibCanProt::getPinDigital(void) {
  unsigned char pin = m_bufferCan[2];
  if (m_debug == true) { Serial.println(pin); }

  unsigned char status = digitalRead(pin);
  returnGetPinDigital(pin,status);
}


void LibCanProt::getPinAnalog(void) {
  unsigned char pin = m_bufferCan[2];
  if (m_debug == true) { Serial.println(pin); }

  unsigned short value = analogRead(pin);

  value = m_valTest++;  //////////////// TEMPORAIRE /////////////////////

  returnGetPinAnalog(pin,value);
}

void LibCanProt::returnGetPinDigital(unsigned char pin, unsigned char status) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_GET_PIN_DIGITAL << 4) + 2 + 0x80;
  *p_buf++ = pin;
  *p_buf++ = status;
  mp_canBus->sendMessage(m_bufferCan, p_buf - m_bufferCan);
  displayMessageReponse(BUS_CAN_GET_PIN_DIGITAL);
  displayPinInfo(pin, status);
}


void LibCanProt::returnGetPinAnalog(unsigned char pin, unsigned char value) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_GET_PIN_ANALOGIQUE << 4) + 2 + 0x80;
  *p_buf++ = pin;
  *p_buf++ = value >> 8;
  *p_buf++ = value & 0xFF;
  mp_canBus->sendMessage(m_bufferCan, p_buf - m_bufferCan);
  displayMessageReponse(BUS_CAN_GET_PIN_ANALOGIQUE);
  displayPinInfo(pin, value);
}


//========== Routines d'envoi de messages initiaux  ========

void LibCanProt::sendSetPinDigital( unsigned char pin, unsigned char status ) {
  unsigned char * p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_SET_PIN_DIGITAL<<4)+2;
  *p_buf++ = pin;
  *p_buf++ = status;
  envoiMessage();
  displayPinInfo(pin, status);
}


void LibCanProt::sendSetPinAnalog( unsigned char pin, unsigned short val ) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_SET_PIN_ANALOGIQUE << 4) + 3;
  *p_buf++ = pin;
  *p_buf++ = val >> 8;
  *p_buf++ = val & 0xFF;
  envoiMessage();
  displayPinInfo(pin, val);
}


void LibCanProt::sendSetMoteur1(char vitesseGauche, char vitesseDroite) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_SET_MOTEUR_1 << 4) + 2;
  *p_buf++ = vitesseGauche;
  *p_buf++ = vitesseDroite;
  envoiMessage();
  displayVitessesInfo(vitesseGauche, vitesseDroite);
}


void LibCanProt::sendSetMoteur2(char vitesseGauche, char vitesseDroite) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_SET_MOTEUR_2 << 4) + 2;
  *p_buf++ = vitesseGauche;
  *p_buf++ = vitesseDroite;
  envoiMessage();
  displayVitessesInfo(vitesseGauche, vitesseDroite);
}


void LibCanProt::sendDisplayString( char * message, unsigned char len) {
  /// \todo sendDisplayString
}


int LibCanProt::sendGetPinDigital(unsigned char pin) {
  purgeMessageRecu(); // Pour ne pas traiter un message précédent reçu avec retard

  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_GET_PIN_DIGITAL << 4) + 1;
  *p_buf++ = pin;
  envoiMessage();
  //mp_canBus->sendMessage(m_bufferCan, p_buf - m_bufferCan);
  //displayMessageEnvoye(BUS_CAN_GET_PIN_DIGITAL);
  if (m_debug == true) { Serial.print(pin); Serial.print(" => "); }

  if (attendReponse(100) == false) {
    if (m_debug == true) { Serial.println(" Reponse non recu"); }
    return -1;
  }

  unsigned char pinRecv = m_bufferCan[2];
  unsigned char status  = m_bufferCan[3];

  displayPinInfo(pinRecv, status);
  return status;
}


int LibCanProt::sendGetPinAnalog(unsigned char pin) {
  purgeMessageRecu(); // Pour ne pas traiter un message précédent reçu avec retard

  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = (BUS_CAN_GET_PIN_ANALOGIQUE << 4) + 1;
  *p_buf++ = pin;
  envoiMessage();
  //mp_canBus->sendMessage(m_bufferCan, p_buf - m_bufferCan);
  //displayMessageEnvoye(BUS_CAN_GET_PIN_ANALOGIQUE);

  if (m_debug == true) { Serial.print(pin); Serial.print(" => "); }

  if (attendReponse(100) == false) {
    if (m_debug == true) { Serial.println(" Reponse non recu"); }
    return -1;
  }

  unsigned char pinRecv;
  int value;

  pinRecv = m_bufferCan[2];
  value   = m_bufferCan[3]<<8;
  value  += m_bufferCan[4];

  displayPinInfo( pinRecv, value);
  return value;
}


//========== Routines de gestion des messages ========

void LibCanProt::envoiMessage(void) {
  unsigned char cmd = (m_bufferCan[1] >> 4) & 0x07;
  unsigned char rep =  m_bufferCan[1] >> 7;
  unsigned char len = (m_bufferCan[1] & 0x0F) + 2;
  
  mp_canBus->sendMessage(m_bufferCan, len);

  Serial.print( rep==0 ? "CAN send=> " : "CAN repo=> ");
  displayMessageString(cmd);
}


bool LibCanProt::attendReponse(int timeOut) {
  unsigned char len;
  while (timeOut > 0) {
    if (mp_canBus->getMessage(m_bufferCan) != 0) {
      return true;
    }
    delay(10);
    timeOut--;
  }
  return false;
}


void LibCanProt::purgeMessageRecu(void) {
  while (mp_canBus->getMessage(m_bufferCan) != 0)
    if(m_debug== true) Serial.println("CAN purge");
}


//========== Routines d'affichage de messages de debug ========

void LibCanProt::displayPinInfo(unsigned char pin, unsigned int  val) {
  if (m_debug == false) return;
  Serial.print("pin");
  Serial.print(pin);
  Serial.print("=");
  Serial.println(val);
}


void LibCanProt::displayVitessesInfo(char vitesseGauche, char vitesseDroite) {
  if (m_debug == false) return;
  Serial.print("Vitesses=");
  Serial.print((int)vitesseGauche);
  Serial.print(":");
  Serial.println((int)vitesseDroite);
}


void LibCanProt::displayMessageReponse(unsigned char cmd) {
  if (m_debug == false) return;
  Serial.print("CAN repo=> ");
  displayMessageString(cmd);
}


void LibCanProt::displayMessageString( unsigned char cmd ) {
  switch(cmd) {
    case BUS_CAN_SET_PIN_DIGITAL    : Serial.print("SET_PIN_DIGITAL ");    break;
    case BUS_CAN_GET_PIN_DIGITAL    : Serial.print("GET_PIN_DIGITAL ");    break;
    case BUS_CAN_SET_PIN_ANALOGIQUE : Serial.print("SET_PIN_ANALOGIQUE "); break;
    case BUS_CAN_GET_PIN_ANALOGIQUE : Serial.print("GET_PIN_ANALOGIQUE "); break;
    case BUS_CAN_SET_MOTEUR_1       : Serial.print("SET_MOTEUR_1 ");       break;
    case BUS_CAN_SET_MOTEUR_2       : Serial.print("SET_MOTEUR_2 ");       break;
    case BUS_CAN_DISPLAY_STRING     : Serial.print("DISPLAY_STRING ");     break;
    case BUS_CAN_CUSTOM_COMMAND     : Serial.print("CUSTOM_COMMAND ");     break;
    default :                         Serial.print("!! INCONNUE !!");      break;
  }
}


void LibCanProt::setDebug(bool debug) { m_debug = debug; }

