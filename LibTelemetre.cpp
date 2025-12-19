/*!
 * \file 	LibUltrason.cpp
 * \brief 	Le fichier source des classes de mesure de distance
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#include "LibTelemetre.h"


//=====================================
// Class LibUltrason
//=====================================

LibUltrason::LibUltrason(unsigned char pinTrig, unsigned char pinEcho, unsigned short distanceMax) {
  pinMode(m_pinTrig = pinTrig, OUTPUT);
  pinMode(m_pinEcho = pinEcho, INPUT);
  setMaxDistance(distanceMax); // On transforme la distance en une durée
  m_begin = false;
}


void LibUltrason::begin(void) { m_begin = true; }


void LibUltrason::setMaxDistance(unsigned short distanceMax) {
  // On traduit le timeout en multipliant par 5.8
  if(distanceMax < 500) distanceMax = 500;
  m_timeOut = distanceMax;
  m_timeOut *=58;
  m_timeOut /=10;
}


unsigned short LibUltrason::getDistance(void) {
  if(m_begin == false ) return 0;
  
  // On mesure le temps de réponse que l'on divise par 5.8
  unsigned long distance = getPulseDuration();
  distance *= 10;
  distance /= 58;

  return (unsigned short) distance;
}

unsigned long LibUltrason::getPulseDuration() {
  // cache the port and bit of the pin in order to speed up the
  // pulse width measuring loop and achieve finer resolution.  
  // calling digitalRead() instead yields much coarser resolution.
  uint8_t bit = digitalPinToBitMask(m_pinEcho);
  uint8_t port = digitalPinToPort(m_pinEcho);

  unsigned long timeStart;

  // wait for any previous pulse to end
  timeStart = micros();
  while ((*portInputRegister(port) & bit) != 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  // On genère un pulse de 10µs sur la pin de Trigger
  digitalWrite(m_pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(m_pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_pinTrig, LOW);

  // wait for the pulse to start
  while ((*portInputRegister(port) & bit) == 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  // Now measure the echo pulse
  timeStart = micros(); // Reinit the time reference
  while ((*portInputRegister(port) & bit) != 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  return micros()-timeStart;
}


//=====================================
// Class LibVl53lox
//=====================================

#define DEFAULT_VL53L0X_MAX_DISTANCE 500

LibVl53lox::LibVl53lox(void) : lox() {
  m_begin = false;
  m_debug = false;
  m_distanceMax = DEFAULT_VL53L0X_MAX_DISTANCE;
}


void LibVl53lox::begin(void) { 

  if(!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    return;
  }

  m_begin = true;
}


void LibVl53lox::setMaxDistance(unsigned short distanceMax) {
  m_distanceMax = distanceMax;
}


unsigned short LibVl53lox::getDistance(void) {
  if(m_begin == false ) return 0;

  lox.rangingTest(&m_measure, false);

  if( m_measure.RangeStatus == 4 ) {
    if(m_debug == true) Serial.println("LibVl53lox => Out of range");
    return m_distanceMax;
  }

  unsigned short distance = (unsigned short) m_measure.RangeMilliMeter;

  distance = ( distance>m_distanceMax ? m_distanceMax : distance );
  if(m_debug == true) { Serial.print("LibVl53lox => Distance (mm) = "); Serial.println(distance); }

  return (unsigned short) distance;
}


void LibVl53lox::setDebug(bool debug) { m_debug = debug; Serial.println("Trace LibVl53lox activée"); }


