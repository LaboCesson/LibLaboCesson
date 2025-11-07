/*!
 * \file 	LibMcp23008.cpp
 * \brief 	Le fichier source de la classe LibMcp23008
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibMcp23008.h"

#define MCP23008_IODIR   0x00
#define MCP23008_IOPOL   0x01
#define MCP23008_GPINTEN 0x02
#define MCP23008_DEFVAL  0x03
#define MCP23008_INTCON  0x04
#define MCP23008_IOCON   0x05
#define MCP23008_GPPU    0x06
#define MCP23008_INTF    0x07
#define MCP23008_INTCAP  0x08
#define MCP23008_GPIO    0x09
#define MCP23008_OLAT    0x0A

#define MCP23008_NB_REG (MCP23008_OLAT+1)

#define MCP23008_IOCON_SREAD  0x20
#define MCP23008_IOCON_DISSLW 0x10
#define MCP23008_IOCON_ODR    0x04
#define MCP23008_IOCON_INTPOL 0x02

LibMcp23008::LibMcp23008(unsigned char addrDevice, bool dowWireBegin) {
  if(dowWireBegin == true) Wire.begin();
  m_addrDevice = addrDevice;
}


bool LibMcp23008::begin() {
  if( isConnected() == false ) {
    Serial.println("Connection to MCP23008 failed !!");
    return false;
  }
  #define MCP23017_IOCON       0x0A                   // TEMPORAIRE
  #define MCP23017_IOCON_VALUE 0x80                   // TEMPORAIRE
  writeRegister(MCP23017_IOCON, MCP23017_IOCON_VALUE);// TEMPORAIRE

  unsigned char dataReset[]={0xFF,0,0,0,0,0x80,0,0,0,0,0};

  for(unsigned char i=0; i<MCP23008_NB_REG; i++) {
    writeRegister(i,dataReset[i]);
  }
}


unsigned char LibMcp23008::getDeviceRegister(unsigned char * p_reg, unsigned char nbReg) {
  nbReg = ( nbReg > MCP23008_NB_REG  ? MCP23008_NB_REG : nbReg );

  for(unsigned char i=0; i<nbReg; i++) {
    *p_reg++ = readRegister(i);
  }

  return nbReg;
}


void LibMcp23008::pinMode( unsigned char pin, unsigned char mode) {
  if ((pin < 1) || (pin > 8)) return;
 
  unsigned char mask = 1<<(pin-1);
  unsigned char dir = readRegister(MCP23008_IODIR);
  unsigned char olddir = dir;
  unsigned char pup = readRegister(MCP23008_GPPU);
  unsigned char oldpup = pup;

  switch(mode) {
    case INPUT :        dir |=  mask; pup &= ~mask; break;
    case INPUT_PULLUP : dir |=  mask; pup |=  mask; break;
    case OUTPUT :       dir &= ~mask; break;
    default : return;
  }
  if( dir != olddir ) writeRegister(MCP23008_IODIR, dir);
  if( pup != oldpup ) writeRegister(MCP23008_GPPU,  pup);
}


void LibMcp23008::digitalWrite( unsigned char pin, unsigned char value) {
  if ((pin < 1) || (pin > 8)) return;

  unsigned char mask = 1<< (pin - 1);
  unsigned char val = readRegister(MCP23008_GPIO);
  unsigned char oldval = val;

  switch(value) {
    case LOW  : val &= ~mask; break;
    case HIGH : val |=  mask; break;
    default : return;
  }
  if( val != oldval ) writeRegister(MCP23008_GPIO, val);
}


unsigned char LibMcp23008::digitalRead( unsigned char pin) {
  if ((pin < 1) || (pin > 8)) return LOW;
 
  unsigned char mask = 1<< (pin - 1);
  unsigned char val = readRegister(MCP23008_GPIO);

  return ((val&mask) == 0 ? LOW : HIGH );
}


bool LibMcp23008::isConnected() {
  Wire.beginTransmission(m_addrDevice);
  if(Wire.endTransmission() != 0) return false;
  return true;
}


void LibMcp23008::writeRegister(unsigned char reg, unsigned char data) {
  Wire.beginTransmission(m_addrDevice);
  Wire.write(reg);    
  Wire.write(data);  
  Wire.endTransmission();
}


unsigned char LibMcp23008::readRegister(unsigned char reg) {
  unsigned char data;

  Wire.beginTransmission(m_addrDevice);
  Wire.write(reg);    
 	Wire.endTransmission();

	Wire.requestFrom(m_addrDevice, (unsigned char) 1);
 	data = Wire.read();
	return data;
}

