/*!
 * \file 	LibList.cpp
 * \brief 	Le fichier source des classes de la classes LibDistance
 * \author  LaboCesson
 * \version 1.0
 * \date    Avril 2026
 */

#include "LibDistance.h"

#define LIBDISTANCE_ANTIREBOND      5 // Duréee possible de rebond du détecteur en ms
#define LIBDISTANCE_DEFAULT_SPEED 220 // Vitesse par défaut pour les distances courtes en mm/sec


static t_LibDistance_work libDistanceWorkGauche;
static t_LibDistance_work libDistanceWorkDroite;


LibDistance::LibDistance(unsigned int pinRoueGauche, unsigned int pinRoueDroite) {
	m_pinRoueGauche = pinRoueGauche;
	m_pinRoueDroite = pinRoueDroite;
  m_nbPoint       = 1; // pour éviter des divisions par 0
}


void LibDistance::begin(unsigned int diametre, unsigned int nbPoint, bool levelDetector, unsigned int dureeAntirebond ) {
  m_diametre = diametre;
  m_nbPoint  = nbPoint;
  m_levelDetector   = levelDetector;
  m_dureeAntirebond = dureeAntirebond;
  m_begin    = true;

  if (m_nbPoint <= 0) m_nbPoint = 1; // pour éviter des divisions par 0
 
  if (m_pinRoueGauche != 0) {
    pinMode(m_pinRoueGauche, INPUT_PULLUP);
    initCtxRoue(&libDistanceWorkGauche, m_pinRoueGauche);
    attachInterrupt(m_pinRoueGauche, gestionIntRoueGauche, CHANGE);
  }

  if (m_pinRoueDroite != 0) {
    pinMode(m_pinRoueDroite, INPUT_PULLUP);
    initCtxRoue(&libDistanceWorkDroite, m_pinRoueDroite);
    attachInterrupt(m_pinRoueDroite, gestionIntRoueDroite, CHANGE);
  }

  m_defaultSpeed = LIBDISTANCE_DEFAULT_SPEED;
}

void LibDistance::initCtxRoue( t_LibDistance_work * p_ctx, unsigned int pinDetector ) {
  // Avant d'initiaiser le contexte si le parcours précédent a pu déteminer une vitesse
  // On va la considérer comme nouvelle valeur par défaut
  if (p_ctx->firstVitesse != 0) {
    m_defaultSpeed = p_ctx->firstVitesse;
  }

	p_ctx->nbPoint         = m_nbPoint;
  p_ctx->pinDetector     = pinDetector;
  p_ctx->circonference   = (m_diametre * 314)/10;
	p_ctx->sectionSize     = p_ctx->circonference/m_nbPoint;
  p_ctx->levelDetector   = m_levelDetector;
  p_ctx->dureeAntirebond = m_dureeAntirebond;

  for(int i=0; i<LIBDISTANCE_MAX_SECTION; i++ ) p_ctx->detectionTime[i]=0;
  p_ctx->indexTable = 0;
	p_ctx->nbTime     = 0;

  p_ctx->nbDetection    = 0;
	p_ctx->lastVitesse    = 0;
	p_ctx->firstVitesse   = m_defaultSpeed;
	p_ctx->vitesseMoyenne = 0;
  p_ctx->lastInterrupt     = 0;
  p_ctx->firstSectionTime  = 0;
  p_ctx->lastDetectionTime = 0;
  p_ctx->launchingTime     = millis(); ///A la fin pour etre au plus prêt du lancement des moteurs
}


void LibDistance::startDistance() {
  if (m_pinRoueGauche != 0) {
    startDistanceGauche();
  }
  if (m_pinRoueDroite != 0) {
    startDistanceDroite();
  }
}


void LibDistance::startDistanceGauche() {
  initCtxRoue(&libDistanceWorkGauche, m_pinRoueGauche);
}


void LibDistance::startDistanceDroite() {
  initCtxRoue(&libDistanceWorkDroite, m_pinRoueDroite);
}


unsigned int LibDistance::getDistance() {
  unsigned int nbWheel  = 0;
  unsigned int distance = 0;

  if (m_pinRoueGauche != 0) {
    distance += getWheelDistance(&libDistanceWorkGauche);
    nbWheel++;
  }
  if (m_pinRoueDroite != 0) {
    distance += getWheelDistance(&libDistanceWorkDroite);
    nbWheel++;
  }

  return(nbWheel == 0 ? 0 : distance/ nbWheel);
}


unsigned int LibDistance::getWheelDistance(t_LibDistance_work* p_ctx) {
  if (m_begin == false) return 10000; // On retourne 10m pour etre sur qu'il ne bougera pas

  unsigned long time = millis();
  noInterrupts();
  unsigned long lastDetectionTime = p_ctx->lastDetectionTime;
  unsigned int  nbDetection       = p_ctx->nbDetection;
  interrupts();

  unsigned int  distance = 0;

  if(nbDetection <= 1) {
    // On fait une estimation sur la base d'une vitesse estimée
    //distance += ((time - p_ctx->launchingTime) * 220) / 100;
    distance += ((time - p_ctx->launchingTime) * p_ctx->firstVitesse) / 100;
  }

  if (nbDetection > 1) {
    // On ajoute la distance de la première section incomplète
    distance += (p_ctx->firstSectionTime * p_ctx->firstVitesse) / 100;
    // On ajoute la distance de toute les sections complètes
    distance += ((nbDetection - 1) * p_ctx->sectionSize);
    // On ajoute la distance de la dernière section incomplète
    distance += ((time - lastDetectionTime) * p_ctx->vitesseMoyenne) / 100;
  }

  if (m_debug == true) printInfo(p_ctx, distance);

  return distance/10;
}


void LibDistance::gestionIntRoue( t_LibDistance_work * p_ctx) {
  unsigned long time = millis();

  // On gère un antirebond en ne gérant pas l'interruption avant un certain temps
  //if (time < (p_ctx->lastInterrupt + LIBDISTANCE_ANTIREBOND)) return;
  if (time < (p_ctx->lastInterrupt + p_ctx->dureeAntirebond)) return;

  p_ctx->lastInterrupt = time;

  // On ne gere qu'un seul des deux états
  //if (digitalRead(p_ctx->pinDetector) == LOW) return;
  if (digitalRead(p_ctx->pinDetector) == p_ctx->levelDetector) return;

  p_ctx->nbDetection++;

  if(p_ctx->nbDetection == 1 ) {
    // Cas particulier de la première détection
    p_ctx->lastDetectionTime = time;
    p_ctx->detectionTime[0]  = time;
    p_ctx->indexTable = 1;
    p_ctx->nbTime     = 1;
    return;
  }

  // On calcule la vitesse de la section passée
  p_ctx->lastVitesse = (p_ctx->sectionSize * 100)/(time - p_ctx->lastDetectionTime);

  if(p_ctx->nbDetection == 2 ) {
    // Cas particulier de la seconde détection
    // On va conserver en mémoire le temps avant la première détection
    // Et la vitesse de la première section complète
    p_ctx->firstSectionTime = p_ctx->detectionTime[0]- p_ctx->launchingTime;
    p_ctx->firstVitesse     = p_ctx->lastVitesse;
  }

  // On calcule la vitesse moyenne sur un tour
  if(p_ctx->nbTime < p_ctx->nbPoint ) {
    // On n'a pas encore fait un tour complet
    unsigned long tmpTime = time- p_ctx->detectionTime[0];
    unsigned long tmpSize = ((p_ctx->circonference*100)* p_ctx->nbTime)/ p_ctx->nbPoint;
    p_ctx->vitesseMoyenne = tmpSize/tmpTime;
  } else {
    // On a fait un tour complet
    p_ctx->vitesseMoyenne = (p_ctx->circonference*100)/(time- p_ctx->detectionTime[p_ctx->indexTable]);
  }

  // On range l'heure lue dans la table
  p_ctx->detectionTime[p_ctx->indexTable] = time;
  if(p_ctx->nbTime  < p_ctx->nbPoint) p_ctx->nbTime++;
  p_ctx->indexTable = (p_ctx->indexTable + 1) % p_ctx->nbPoint;

  p_ctx->lastDetectionTime = time;
}


void IRAM_ATTR LibDistance::gestionIntRoueDroite() {
  gestionIntRoue( &libDistanceWorkDroite );
  return;
}


void IRAM_ATTR LibDistance::gestionIntRoueGauche() {
  gestionIntRoue( &libDistanceWorkGauche );
  return;
}


void LibDistance::printInfo(t_LibDistance_work* p_ctx, unsigned int distance) {
  Serial.print("nbD=");        Serial.print(p_ctx->nbDetection);
  Serial.print(" - StartTi="); Serial.print(p_ctx->launchingTime);
  Serial.print(" - lastDet="); Serial.print(p_ctx->lastDetectionTime);
  Serial.print(" - dis=");     Serial.print(distance);
  Serial.print(" - vFirst=");  Serial.print(p_ctx->firstVitesse);
  Serial.print(" - vLast=");   Serial.print(p_ctx->lastVitesse);
  Serial.print(" - vMoy=");    Serial.print(p_ctx->vitesseMoyenne);
  Serial.println("");
}

