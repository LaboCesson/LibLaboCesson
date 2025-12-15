/*!
 * \file 	LibMuxI2c.cpp
 * \brief 	Le fichier source de la classe LibMuxI2c
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#include "LibMuxI2c.h"


LibMuxI2c::LibMuxI2c(unsigned char muxAddress) {
  m_debug = false;
  m_muxAddress = muxAddress;
  m_line = 0;
}


void LibMuxI2c::selectLine(unsigned char i2cLine) {
  if( i2cLine> 7 ) {
    Serial.println("!!! LibMuxI2c : ligne I2C inexistante");
    return;
  }

  m_line = i2cLine;
  if( m_debug == true ) {
    Serial.print("LibMuxI2c : sélection de la ligne I2C numéro ");
    Serial.println(m_line);
  }
  
  Wire.beginTransmission(m_muxAddress);
  Wire.write(1<<i2cLine);
  Wire.endTransmission();
}
