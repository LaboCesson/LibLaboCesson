/*!
 * \file 	LibColor.cpp
 * \brief 	Le fichier source des classes gérant les détecteurs de couleur
 * \author  LaboCesson
 * \version 1.0
 * \date    Novembre 2025
 *
 * \details Ce fichier contient le source des classes suivantes
 * \details - \ref LibTcs3472
 */

#include "LibColor.h"

//=====================================
// Class LibTcs3472
//=====================================

#define TCS_INTEGRATION_TIME TCS34725_INTEGRATIONTIME_240MS
#define PERIOD_GESTION       241

#define SEUIL_BLEU  6000
#define SEUIL_JAUNE 3000
#define MILIEU_TEMPERATURE 4500


LibTcs3472::LibTcs3472(void) :
  tcs(TCS_INTEGRATION_TIME, TCS34725_GAIN_1X)
{
  m_debug = false;
  m_begin = false;

  m_nextTimeGestion = millis();
  m_period = PERIOD_GESTION;

  m_red   = 0;
  m_green = 0;
  m_blue  = 0;
  m_chrominance = 0;
  m_temperature = 0;
}


bool LibTcs3472::begin(void) {
  if (!tcs.begin()) {
    Serial.println("!!! No TCS3472 found");
    return false;
  }

  trace("TCS3472 found");
  m_begin = true;
  return true;
}


void LibTcs3472::gestion(void) {
  if(m_begin == false) return;

  unsigned long newTime = millis();

  if( newTime < m_nextTimeGestion ) return;

  m_chrominance = tcs.read16(TCS34725_CDATAL);
  m_red   = tcs.read16(TCS34725_RDATAL);
  m_green = tcs.read16(TCS34725_GDATAL);
  m_blue  = tcs.read16(TCS34725_BDATAL);
  m_temperature = tcs.calculateColorTemperature_dn40(m_red, m_green, m_blue, m_chrominance);

  // On évite l'accumulation de traitements le cas écheant
  while (newTime >= m_nextTimeGestion) m_nextTimeGestion += m_period;
}


unsigned short LibTcs3472::getTemperature(void) {
  if( m_begin == false ) return 0;

  if( m_debug == true ) {
    Serial.print("TCS3472 => ");
    Serial.print("Color Temperature:"); Serial.print(m_temperature,   DEC);
    Serial.println("");
  }
  return m_temperature;
}


void LibTcs3472::getRGB(unsigned char * p_red, unsigned char * p_green, unsigned char * p_blue) {
  *p_red   = m_red/128;
  *p_green = m_green/128;
  *p_blue  = m_blue/128;

  if( m_debug == true ) {
    Serial.print("TCS3472 => ");
    Serial.print("R:"); Serial.print(*p_red,   DEC); Serial.print(",");
    Serial.print("G:"); Serial.print(*p_green, DEC); Serial.print(",");
    Serial.print("B:"); Serial.print(*p_blue,  DEC); Serial.print(",");
    Serial.println("");
  }
  return;
}


t_pami_color LibTcs3472::getColor(void){
  if( m_begin == false ) return PAMI_COULEUR_INCONNUE;

  return( m_temperature > MILIEU_TEMPERATURE ? PAMI_COULEUR_BLEU : PAMI_COULEUR_JAUNE );
}


void LibTcs3472::trace(char * msg) {
  if( m_debug == true ) Serial.println(msg);
}


void LibTcs3472::setDebug(bool debug) { m_debug = debug; trace("Trace LibTcs3472 activée"); }



//=====================================
// Class LibMultiTcs3472
//=====================================


LibMultiTcs3472::LibMultiTcs3472(void)
{
  m_debug   = false;
  m_begin   = false;
  m_nbColor = 0;
}


bool LibMultiTcs3472::begin(int nbColor, unsigned char * muxIndex) {
  if( m_begin == true) return false;
  
  bool flagError = false;

  m_nbColor = ( nbColor > NB_MAX_MULTI_TCS3472 ? NB_MAX_MULTI_TCS3472 : nbColor) ;

  m_tabColor = new LibTcs3472[m_nbColor];

  for(int i=0; i < m_nbColor; i++ ) {
    m_muxIndex[i] = muxIndex[i];
    m_mux.selectLine(m_muxIndex[i]);
    if (m_tabColor[i].begin() == false) {
      flagError = true;
      Serial.print("pour la ligne indice ");
      Serial.println(m_muxIndex[i]);
    }
  }

  m_begin = true;

  return flagError;
}


void LibMultiTcs3472::gestion(void) {
  for(int i=0; i<m_nbColor; i++ ) {
    m_mux.selectLine(m_muxIndex[i]);
    m_tabColor[i].gestion();
  }
}


void LibMultiTcs3472::getColors( int nbColor, t_pami_color * p_tabColor ) {
  if( nbColor > m_nbColor ) nbColor = m_nbColor;

  for(int i=0; i<nbColor; i++ ) { p_tabColor[i] = m_tabColor[i].getColor(); }
}


void LibMultiTcs3472::trace(char * msg) {
  if( m_debug == true ) Serial.println(msg);
}


void LibMultiTcs3472::setDebug(bool debug) { m_debug = debug; trace("Trace LibMultiTcs3472 activée"); }
