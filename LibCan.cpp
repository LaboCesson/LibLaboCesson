/*!
 * \file 	LibCan.cpp
 * \brief 	Le fichier source de la classe LibCan2515
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#include "LibCan.h"
#include "LibCanProtDef.h"
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
// Class LibCanProtSend
//=====================================


LibCanProtSend::LibCanProtSend()
{
  m_debug = false;
  m_begin = false;
}


bool LibCanProtSend::begin(void) {
  if( mp_canBus->begin() == false ) return false;
  m_begin = true;
  return true;
}


bool LibCanProtSend::sendPing(void) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_PING_ACK;
  envoiMessage();
  canProtCom.displayAck(BUS_CAN_PING_ACK);
  return waitReponse(BUS_CAN_PING_ACK);
}


bool LibCanProtSend::sendSetPinDigital(unsigned char pin, unsigned char status) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_PIN_DIGITAL;
  *p_buf++ = 2;
  *p_buf++ = pin;
  *p_buf++ = status;
  envoiMessage();
  canProtCom.displayPinInfo(pin, status);
  return waitReponse(BUS_CAN_SET_PIN_DIGITAL);
}


bool LibCanProtSend::sendSetPinAnalog( unsigned char pin, unsigned short val ) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_PIN_ANALOGIQUE;
  *p_buf++ = 3;
  *p_buf++ = pin;
  *p_buf++ = val >> 8;
  *p_buf++ = val & 0xFF;
  envoiMessage();
  canProtCom.displayPinInfo(pin, val);
  return waitReponse(BUS_CAN_SET_PIN_ANALOGIQUE);
}


bool LibCanProtSend::sendSetMoteur1(char vitesseGauche, char vitesseDroite) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_MOTEUR_1;
  *p_buf++ = 2;
  *p_buf++ = vitesseGauche;
  *p_buf++ = vitesseDroite;
  envoiMessage();
  canProtCom.displayVitessesInfo(vitesseGauche, vitesseDroite);
  return waitReponse(BUS_CAN_SET_MOTEUR_1);
}


bool LibCanProtSend::sendSetMoteur2(char vitesseGauche, char vitesseDroite) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_MOTEUR_2;
  *p_buf++ = 2;
  *p_buf++ = vitesseGauche;
  *p_buf++ = vitesseDroite;
  envoiMessage();
  canProtCom.displayVitessesInfo(vitesseGauche, vitesseDroite);
  return waitReponse(BUS_CAN_SET_MOTEUR_2);
}


bool LibCanProtSend::sendDisplayString( char * message, unsigned char len) {
  /// \todo sendDisplayString
}


bool LibCanProtSend::sendSetGpioPwm(unsigned char gpio, unsigned char angle) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_GPIO_PWM;
  *p_buf++ = 2;
  *p_buf++ = gpio;
  *p_buf++ = angle;
  envoiMessage();
  canProtCom.displayGpioAngleInfo(gpio, angle);
  return waitReponse(BUS_CAN_SET_GPIO_PWM);
}


bool LibCanProtSend::sendSetRelay(unsigned char index, bool status) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_SET_RELAY;
  *p_buf++ = 2;
  *p_buf++ = index;
  *p_buf++ = status;
  envoiMessage();
  canProtCom.displayRelayStatus(index, status);
  return waitReponse(BUS_CAN_SET_RELAY);
}


int LibCanProtSend::sendGetPinDigital(unsigned char pin) {
  purgeMessageRecu(); // Pour ne pas traiter un message précédent reçu avec retard

  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_PIN_DIGITAL;
  *p_buf++ = 1;
  *p_buf++ = pin;
  envoiMessage();
  if (m_debug == true) { Serial.print(pin); Serial.print(" => "); }

  if (waitReponse(BUS_CAN_GET_PIN_DIGITAL) == false) {
    return -1;
  }

  unsigned char pinRecv = m_bufferCan[3];
  unsigned char status  = m_bufferCan[4];

  canProtCom.displayPinInfo(pinRecv, status);
  return status;
}


int LibCanProtSend::sendGetPinAnalog(unsigned char pin) {
  purgeMessageRecu(); // Pour ne pas traiter un message précédent reçu avec retard

  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_PIN_ANALOGIQUE;
  *p_buf++ = 1;
  *p_buf++ = pin;
  envoiMessage();
  if (m_debug == true) { Serial.print(pin); Serial.print(" => "); }

  if (waitReponse(BUS_CAN_GET_PIN_ANALOGIQUE) == false) {
    return -1;
  }

  unsigned char pinRecv;
  int value;

  pinRecv = m_bufferCan[3];
  value   = m_bufferCan[4]<<8;
  value  += m_bufferCan[5];

  canProtCom.displayPinInfo( pinRecv, value);
  return value;
}


int LibCanProtSend::sendGetColor(unsigned char nbColor ) {
  purgeMessageRecu(); // Pour ne pas traiter un message précédent reçu avec retard
  
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_COLOR;
  *p_buf++ = 1;
  *p_buf++ = nbColor;
  envoiMessage();
  if (m_debug == true) { Serial.print(nbColor); Serial.print(" couleur => "); }

  if (waitReponse(BUS_CAN_GET_COLOR) == false) {
    return -1;
  }

  unsigned char color = m_bufferCan[3];
  canProtCom.displayColorInfo(color);
  return color;
}


//========== Routines de gestion des messages ========

void LibCanProtSend::envoiMessage(void) {
  unsigned char cmd = m_bufferCan[1] & 0x7F;
  unsigned char rep = m_bufferCan[1] >> 7;
  unsigned char len = m_bufferCan[2] + 3;
  
  mp_canBus->sendMessage(m_bufferCan, len);

  if (m_debug == true) {
    Serial.print(rep == 0 ? "CAN send=> " : "CAN repo=> ");
    canProtCom.displayMessageString(cmd);
  }
}


bool LibCanProtSend::waitReponse(unsigned char cmd) {
  int timeOut = BUS_CAN_TIME_OUT;
  unsigned char len;


  while (timeOut > 0) {
    if (mp_canBus->getMessage(m_bufferCan) != 0) {
      if (m_bufferCan[0] != CAN_MAGIC_TAG) {
        if (m_debug == true) { Serial.print(" !!BAD MAGICTAG!! "); Serial.println(m_bufferCan[0], HEX); }
        return false;
      }
      if (m_bufferCan[1] != (cmd | 0x80)) {
        if (m_debug == true) { Serial.print(" !!BAD COMMAND!! "); Serial.println(m_bufferCan[1]&0x7F); }
        return false;
      }
      return true;
    }
    delay(10);
    timeOut--;
  }
  if (m_debug == true) { Serial.println(" Acquitement non recu"); }
  return false;
}


void LibCanProtSend::purgeMessageRecu(void) {
  while (mp_canBus->getMessage(m_bufferCan) != 0)
    if(m_debug== true) Serial.println("CAN purge");
}


void LibCanProtSend::setDebug(bool debug) {
  m_debug = debug; 
  canProtCom.setDebug(debug);
}



//=====================================
// Class LibCanProtRecv
//=====================================

LibCanProtRecv::LibCanProtRecv()
{
  m_debug = false;
  m_begin = false;
}


bool LibCanProtRecv::begin(void) {
  if (mp_canBus->begin() == false) return false;
  m_begin = true;
  return true;
}


void LibCanProtRecv::setMoteurDriver(unsigned char moteur, LibMoteur* p_moteur) {
  switch (moteur) {
  case 0: mp_moteur1 = p_moteur; break;
  case 1: mp_moteur2 = p_moteur; break;
  }
}


void LibCanProtRecv::setPinRelayList(unsigned char nbRelay, unsigned char* pinRelay) {
  m_nbRelay = (nbRelay > CAN_BUS_MAX_RELAY ? CAN_BUS_MAX_RELAY : nbRelay);
  for (int i = 0; i < m_nbRelay; i++) {
    m_pinRelay[i] = pinRelay[i];
    pinMode(pinRelay[i], OUTPUT);
    digitalWrite(pinRelay[i], HIGH);
  }
}


bool LibCanProtRecv::gestionMessage(void) {
  if (m_begin == false) return false;

  unsigned char len = mp_canBus->getMessage(m_bufferCan);

  if (len == 0) return false;

  unsigned char cmd = m_bufferCan[1];

  if (m_debug == true) {
    Serial.print("CAN recv=> ");
    canProtCom.displayMessageString(cmd);
  }

  switch (cmd) {
    case BUS_CAN_PING_ACK:           ping();          break;
    case BUS_CAN_SET_PIN_DIGITAL:    setPinDigital(); break;
    case BUS_CAN_GET_PIN_DIGITAL:    getPinDigital(); break;
    case BUS_CAN_SET_PIN_ANALOGIQUE: setPinAnalog();  break;
    case BUS_CAN_GET_PIN_ANALOGIQUE: getPinAnalog();  break;
    case BUS_CAN_SET_MOTEUR_1:       setMoteur(BUS_CAN_SET_MOTEUR_1); break;
    case BUS_CAN_SET_MOTEUR_2:       setMoteur(BUS_CAN_SET_MOTEUR_2); break;
    case BUS_CAN_DISPLAY_STRING:     displayString(); break;
    case BUS_CAN_GET_COLOR:          getColor();      break;
    case BUS_CAN_SET_GPIO_PWM:       setGpioPwm();    break;
    case BUS_CAN_SET_RELAY:          setRelay();      break;
    case BUS_CAN_CUSTOM_COMMAND: break;
    default: Serial.print(cmd); Serial.println(""); return false;
  }
  return true;
}


//========== Routines de traitement des messages reçus  ========


void LibCanProtRecv::ping(void) {
  canProtCom.displayAck(BUS_CAN_PING_ACK);
  returnAck(BUS_CAN_PING_ACK);
}


void LibCanProtRecv::setPinDigital(void) {
  unsigned char pin = m_bufferCan[3];
  unsigned char status = m_bufferCan[4];
  digitalWrite(pin, status);
  canProtCom.displayPinInfo(pin, status);
  returnAck(BUS_CAN_SET_PIN_DIGITAL);
}


void LibCanProtRecv::setPinAnalog(void) {
  unsigned char  pin = m_bufferCan[3];
  unsigned short val = m_bufferCan[4] & 0x03;
  val <<= 8;
  val += m_bufferCan[5];
  analogWrite(pin, val);
  canProtCom.displayPinInfo(pin, val);
  returnAck(BUS_CAN_SET_PIN_ANALOGIQUE);
}


void LibCanProtRecv::setMoteur(unsigned char cmd) {
  LibMoteur* p_moteur;

  switch (cmd) {
    case BUS_CAN_SET_MOTEUR_1: p_moteur = mp_moteur1; break;
    case BUS_CAN_SET_MOTEUR_2: p_moteur = mp_moteur2; break;
    default: return;
  }

  if (p_moteur == NULL) {
    Serial.println("CAN recv=> !!MOTEUR INEXISTANT");
    return;
  }

  char vitesseGauche = m_bufferCan[3];
  char vitesseDroite = m_bufferCan[4];

  p_moteur->moteurGauche(vitesseGauche);
  p_moteur->moteurDroit(vitesseDroite);

  canProtCom.displayVitessesInfo(vitesseGauche, vitesseDroite);
  returnAck(cmd);
}


void LibCanProtRecv::displayString(void) {
  // afficheur.displayBuffer(&(p_buf[4]), p_buf[3]);
}


void LibCanProtRecv::getPinDigital(void) {
  unsigned char pin = m_bufferCan[3];
  if (m_debug == true) { Serial.println(pin); }

  unsigned char status = digitalRead(pin);
  returnGetPinDigital(pin, status);
}


void LibCanProtRecv::getPinAnalog(void) {
  unsigned char pin = m_bufferCan[3];
  if (m_debug == true) { Serial.println(pin); }

  unsigned short value = analogRead(pin);
  returnGetPinAnalog(pin, value);
}


void LibCanProtRecv::getColor(void) {
  unsigned char nbColor = m_bufferCan[3];
  unsigned char color = 0xFF;
  t_robot_color tabColor[4];

  if (nbColor > 4) nbColor = 4;

  if (m_debug == true) { Serial.print(nbColor); Serial.println(" couleur"); }

  if (mp_color != NULL) {
    color <<= 2;
    color += (mp_color->getColor() & 0x03);
  }
  else if (mp_color_multi != NULL) {
    mp_color_multi->getColors(nbColor, tabColor);
    for (int i = (nbColor - 1); i>=0; i--) {
      color <<= 2;
      color += (tabColor[i] & 0x03);
    }
  }
  else {
    Serial.println("CANbus !! Gestionnaire Color non initialise !!");
  }

  returnGetColor(color);
}


void LibCanProtRecv::setGpioPwm(void) {
  unsigned char gpio = m_bufferCan[3];
  unsigned char angle = m_bufferCan[4];
  if (mp_gpio == NULL) {
    Serial.println("CANbus !! Gestionnaire GPIO non initialise !!");
  }

  mp_gpio->set(gpio-1, angle);
  canProtCom.displayGpioAngleInfo(gpio, angle);
  returnAck(BUS_CAN_SET_GPIO_PWM);
}


void LibCanProtRecv::setRelay(void) {
  unsigned char relay  = m_bufferCan[3];
  bool          status = m_bufferCan[4];

  digitalWrite(m_pinRelay[relay - 1], (status == LOW ? HIGH : LOW));
  canProtCom.displayRelayStatus(relay, status);
  returnAck(BUS_CAN_SET_RELAY);
}


void LibCanProtRecv::returnAck(unsigned char cmd) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = cmd + 0x80;
  envoiMessage();
  canProtCom.displayAck(cmd);
}


void LibCanProtRecv::returnGetPinDigital(unsigned char pin, unsigned char status) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_PIN_DIGITAL + 0x80;
  *p_buf++ = 2;
  *p_buf++ = pin;
  *p_buf++ = status;
  envoiMessage();
  canProtCom.displayPinInfo(pin, status);
}


void LibCanProtRecv::returnGetPinAnalog(unsigned char pin, unsigned char value) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_PIN_ANALOGIQUE + 0x80;
  *p_buf++ = 3;
  *p_buf++ = pin;
  *p_buf++ = value >> 8;
  *p_buf++ = value & 0xFF;
  envoiMessage();
  canProtCom.displayPinInfo(pin, value);
}


void LibCanProtRecv::returnGetColor(unsigned char color) {
  unsigned char* p_buf = m_bufferCan;
  *p_buf++ = CAN_MAGIC_TAG;
  *p_buf++ = BUS_CAN_GET_COLOR + 0x80;
  *p_buf++ = 1;
  *p_buf++ = color;
  envoiMessage();
  canProtCom.displayColorInfo(color);
}


//========== Routines de gestion des messages ========

void LibCanProtRecv::envoiMessage(void) {
  unsigned char cmd = m_bufferCan[1] & 0x7F;
  unsigned char rep = m_bufferCan[1] >> 7;
  unsigned char len = m_bufferCan[2] + 3;

  mp_canBus->sendMessage(m_bufferCan, len);

  if (m_debug == true) {
    Serial.print(rep == 0 ? "CAN send=> " : "CAN repo=> ");
    canProtCom.displayMessageString(cmd);
  }
}


void LibCanProtRecv::purgeMessageRecu(void) {
  while (mp_canBus->getMessage(m_bufferCan) != 0)
    if (m_debug == true) Serial.println("CAN purge");
}


void LibCanProtRecv::setDebug(bool debug) {
  m_debug = debug;
  canProtCom.setDebug(debug);
}



//=====================================
// Class LibCanProtCommon
//=====================================

void LibCanProtCommon::displayAck(unsigned char cmd) {
  if (m_debug == false) return;
  Serial.println("Ack ");
}


void LibCanProtCommon::displayPinInfo(unsigned char pin, unsigned int  val) {
  if (m_debug == false) return;
  Serial.print("pin");
  Serial.print(pin);
  Serial.print("=");
  Serial.println(val);
}


void LibCanProtCommon::displayVitessesInfo(char vitesseGauche, char vitesseDroite) {
  if (m_debug == false) return;
  Serial.print("Vitesses=");
  Serial.print((int)vitesseGauche);
  Serial.print(":");
  Serial.println((int)vitesseDroite);
}


void LibCanProtCommon::displayColorInfo(unsigned int color) {
  if (m_debug == false) return;
  for (int i = 0; i < 4; i++)
  {
    switch (color & 0x03) {
    case  ROBOT_COULEUR_INCONNUE: Serial.print("?"); break;
    case  ROBOT_COULEUR_JAUNE: Serial.print("J"); break;
    case  ROBOT_COULEUR_BLEU: Serial.print("B"); break;
    default: Serial.print("-"); break;
    }
    color >>= 2;
  }
  Serial.println("");
}


void LibCanProtCommon::displayGpioAngleInfo(unsigned char gpio, unsigned char angle) {
  if (m_debug == false) return;
  Serial.print(gpio);
  Serial.print(":");
  Serial.print(angle);
  Serial.println("°");
}


void LibCanProtCommon::displayRelayStatus(unsigned char relay, bool status) {
  if (m_debug == false) return;
  Serial.print(relay);
  Serial.print(":");
  Serial.println(status == LOW ? "Ouvert" :"Fermé");
}


void LibCanProtCommon::displayMessageString(unsigned char cmd) {
  switch (cmd) {
  case BUS_CAN_PING_ACK:           Serial.print("PING/ACK ");           break;
  case BUS_CAN_SET_PIN_DIGITAL:    Serial.print("SET_PIN_DIGITAL ");    break;
  case BUS_CAN_GET_PIN_DIGITAL:    Serial.print("GET_PIN_DIGITAL ");    break;
  case BUS_CAN_SET_PIN_ANALOGIQUE: Serial.print("SET_PIN_ANALOGIQUE "); break;
  case BUS_CAN_GET_PIN_ANALOGIQUE: Serial.print("GET_PIN_ANALOGIQUE "); break;
  case BUS_CAN_SET_MOTEUR_1:       Serial.print("SET_MOTEUR_1 ");       break;
  case BUS_CAN_SET_MOTEUR_2:       Serial.print("SET_MOTEUR_2 ");       break;
  case BUS_CAN_DISPLAY_STRING:     Serial.print("DISPLAY_STRING ");     break;
  case BUS_CAN_GET_COLOR:          Serial.print("BUS_CAN_GET_COLOR ");  break;
  case BUS_CAN_SET_GPIO_PWM:       Serial.print("BUS_SET_GPIO_PWM ");   break;
  case BUS_CAN_SET_RELAY:          Serial.print("BUS_SET_RELAY ");      break;
  case BUS_CAN_CUSTOM_COMMAND:     Serial.print("CUSTOM_COMMAND ");     break;
  default:                         Serial.print("!! INCONNUE !! ");     break;
  }
}

