/*!
 * \file 	LibMoteur.cpp
 * \brief 	Le fichier source de la classe LibMoteur
 * \author  LaboCesson
 * \version 2.0
 * \date    Octobre 2025
 */

#include "LibMoteur.h"

#define DEFAULT_FREQUENCY 1000
#define DEFAULT_RESOLUTION   8
#define DEFAULT_BASE_CHANNEL 4


#define DEFAULT_SERVO_MIN 1000
#define DEFAULT_SERVO_OFF 1500
#define DEFAULT_SERVO_MAX 2000


//=====================================
// Class LibMoteur
//=====================================

LibMoteur::LibMoteur(unsigned char pinIn1, unsigned char pinIn2, unsigned char pinIn3, unsigned char pinIn4 ) {
  commonInit();

  m_typeDriver   = MOTEUR_L298N_4;

  m_frequency    = DEFAULT_FREQUENCY;
  m_resolution   = DEFAULT_RESOLUTION;

  #ifdef AVR
    pinMode(m_avantGauche   = pinIn1, OUTPUT); analogWrite(m_avantGauche,  0);
    pinMode(m_arriereGauche = pinIn2, OUTPUT); analogWrite(m_arriereGauche,0);
    pinMode(m_arriereDroite = pinIn3, OUTPUT); analogWrite(m_arriereDroite,0);
    pinMode(m_avantDroite   = pinIn4, OUTPUT); analogWrite(m_avantDroite,  0);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    m_avantGauche   = pinIn1;
    m_arriereGauche = pinIn2;
    m_arriereDroite = pinIn3;
    m_avantDroite   = pinIn4;

    m_attachStatus[MOTEUR_GAUCHE] = NO_ATTACH;
    m_attachStatus[MOTEUR_DROIT]  = NO_ATTACH;
  #endif
}


LibMoteur::LibMoteur(unsigned char pinEna, unsigned char pinIn1, unsigned char pinIn2,
                     unsigned char pinIn3, unsigned char pinIn4, unsigned char pinEnb) {
  commonInit();

  m_typeDriver = MOTEUR_L298N_6;

  m_frequency  = DEFAULT_FREQUENCY;
  m_resolution = DEFAULT_RESOLUTION;

  pinMode(m_avantGauche   = pinIn1, OUTPUT); digitalWrite(m_avantGauche, LOW);
  pinMode(m_arriereGauche = pinIn2, OUTPUT); digitalWrite(m_arriereGauche, LOW);
  pinMode(m_arriereDroite = pinIn3, OUTPUT); digitalWrite(m_arriereDroite, LOW);
  pinMode(m_avantDroite   = pinIn4, OUTPUT); digitalWrite(m_avantDroite, LOW);

  #ifdef AVR
    pinMode(m_enableGauche = pinEna, OUTPUT); analogWrite(m_enableGauche, 0);
    pinMode(m_enableDroite = pinEnb, OUTPUT); analogWrite(m_enableDroite, 0);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    ledcAttach(m_enableGauche, m_frequency, m_resolution); ledcWrite(m_enableGauche, 0);
    ledcAttach(m_enableDroite, m_frequency, m_resolution); ledcWrite(m_enableDroite, 0);
  #endif
}


LibMoteur::LibMoteur(unsigned char pinGauche, unsigned char pinDroite) {
  commonInit();

  m_pinGauche = pinGauche;
  m_pinDroite = pinDroite;
  m_typeDriver = MOTEUR_SERVO;
  return;

  mp_servoGauche = new Servo();
  mp_servoGauche->attach(m_pinGauche, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  mp_servoGauche->writeMicroseconds(DEFAULT_SERVO_OFF);

  mp_servoDroit = new Servo();
  mp_servoDroit->attach(m_pinDroite, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  mp_servoDroit->writeMicroseconds(DEFAULT_SERVO_OFF);

  m_begin = true;
}


void LibMoteur::commonInit(void) {
  m_lastVitesseGauche = m_lastVitesseDroite = 0;
  m_directionGauche = m_directionDroite = false;
}


void LibMoteur::begin(void) {
  if (m_begin == true) return;
  switch (m_typeDriver) {
  case MOTEUR_L298N_4: break;
  case MOTEUR_L298N_6: break;
  case MOTEUR_SERVO:
      mp_servoGauche = new Servo();
      mp_servoGauche->attach(m_pinGauche, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
      mp_servoGauche->writeMicroseconds(DEFAULT_SERVO_OFF);

      mp_servoDroit = new Servo();
      mp_servoDroit->attach(m_pinDroite, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
      mp_servoDroit->writeMicroseconds(DEFAULT_SERVO_OFF);

      break;
    default: break;
  }
  m_begin = true;
}


void LibMoteur::moteurs(int vitesse) {
  moteurGauche(vitesse);
  moteurDroit(vitesse);
}


void LibMoteur::moteurGauche(int vitesse) {
  if (m_begin == false) return;
  if (m_directionGauche == true) vitesse = -vitesse;
  if (vitesse == m_lastVitesseGauche) return;
  m_lastVitesseGauche = vitesse;
  trace("gauche", vitesse);

  switch (m_typeDriver) {
    case MOTEUR_L298N_4 : setVitesseMoteurL298n_4(vitesse, m_avantGauche, m_arriereGauche, MOTEUR_GAUCHE); break;
    case MOTEUR_L298N_6 : setVitesseMoteurL298n_6(vitesse, m_enableGauche,m_avantGauche, m_arriereGauche, MOTEUR_GAUCHE); break;
    case MOTEUR_SERVO   : setVitesseMoteurServo(vitesse, mp_servoGauche ); break;
    default: break;
  }
}


void LibMoteur::moteurDroit(int vitesse) {
  if (m_begin == false) return;
  if (m_directionDroite == true) vitesse = -vitesse;
  if (vitesse == m_lastVitesseDroite) return;
  m_lastVitesseDroite = vitesse;
  trace("droit", vitesse);

  switch (m_typeDriver) {
    case MOTEUR_L298N_4 : setVitesseMoteurL298n_4(vitesse, m_avantDroite, m_arriereDroite, MOTEUR_DROIT); break;
    case MOTEUR_L298N_6 : setVitesseMoteurL298n_6(vitesse, m_enableDroite,m_avantDroite, m_arriereDroite, MOTEUR_DROIT); break;
    case MOTEUR_SERVO   : setVitesseMoteurServo(vitesse, mp_servoDroit); break;
    default: break;
  }
}


void LibMoteur::setVitesseMoteurL298n_4( int vitesse, unsigned char pinAvant, unsigned char pinArriere, t_indexMoteur indexMoteur) {
  unsigned char vAv = 0; 
  unsigned char vAr = 0;

  if (vitesse > 0) vAv = map( vitesse, 0, 100, 0, 255);
  if (vitesse < 0) vAr = map(-vitesse, 0, 100, 0, 255);

  #ifdef AVR
    analogWrite(pinAvant,  vAv);
    analogWrite(pinArriere,vAr);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    bool status;

    // On désalloue un éventuel canal plus utilisé
    if ((vAv == 0) && (m_attachStatus[indexMoteur] == AV_ATTACH)) {
      ledcDetach(pinAvant);
      m_attachStatus[indexMoteur] = NO_ATTACH;
    }
    if ((vAr == 0) && (m_attachStatus[indexMoteur] == AR_ATTACH)) {
      ledcDetach(pinArriere);
      m_attachStatus[indexMoteur] = NO_ATTACH;
    }

    // On alloue maintenant un canal si besoin
    if (vAv != 0) {
      if (m_attachStatus[indexMoteur] != AV_ATTACH) {
        status = ledcAttach(pinAvant, m_frequency, m_resolution);
        if (status == false) Serial.println("LibMoteur => !! Pas assez de canal PWM !!");
        else m_attachStatus[indexMoteur] = AV_ATTACH;
      }
      ledcWrite(pinAvant, vAv);
    }

    if (vAr != 0) {
      if (m_attachStatus[indexMoteur] != AR_ATTACH) {
        status = ledcAttach(pinArriere, m_frequency, m_resolution);
        if (status == false) Serial.println("LibMoteur => !! Pas assez de canal PWM !!");
        else m_attachStatus[indexMoteur] = AR_ATTACH;
      }
      ledcWrite(pinArriere, vAr);
    }
  #endif
}


void LibMoteur::setVitesseMoteurL298n_6(int vitesse, unsigned char pinVitesse, unsigned char pinAvant, unsigned char pinArriere, t_indexMoteur indexMoteur) {
  unsigned char v; // vitesse
  v = (vitesse > 0 ? vitesse : -vitesse);
  v = map(v, 0, 100, 0, 255);
  if (vitesse == 0) {
    // On est à l'arret
    digitalWrite(pinAvant,  LOW);
    digitalWrite(pinArriere, LOW);
  }
  else if (vitesse > 0) {
    // On va vers l'avant
    digitalWrite(pinArriere, LOW);
    digitalWrite(pinAvant,   HIGH);
  }
  else {
    // On va vers l'arriere
    digitalWrite(pinAvant,   LOW);
    digitalWrite(pinArriere, HIGH);
  }

  #ifdef AVR
    analogWrite(pinVitesse, v);
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    ledcWrite(pinVitesse, v);
  #endif

}



void LibMoteur::setVitesseMoteurServo(int vitesse, Servo* p_servo) {
  unsigned short value = DEFAULT_SERVO_OFF;

  if (vitesse != 0) {
    value = map(vitesse, -100, 100, DEFAULT_SERVO_MIN, DEFAULT_SERVO_MAX);
  }
 
  p_servo->writeMicroseconds(value);
}


void LibMoteur::setDirection(bool dirGauche, bool dirDroite) {
  m_directionGauche = dirGauche;
  m_directionDroite = dirDroite;

  if ((dirGauche != 1) && (dirGauche != -1)) dirGauche = 1;
  if ((dirDroite != 1) && (dirDroite != -1)) dirDroite = 1;

}


void LibMoteur::trace(char* side, int vitesse) {
  if (m_debug == false) return;
  Serial.print("Moteur ");
  Serial.print(side);
  Serial.print(":");
  Serial.println((int)vitesse);
}


void LibMoteur::setDebug(bool debug) { m_debug = debug; }


//=====================================
// Class LibMoteurS
//=====================================


LibMoteurS::LibMoteurS(unsigned char pinGauche, unsigned char pinDroit, 
                       unsigned short stepTime, unsigned char  stepUp, unsigned char  stepDown) :
  moteur(pinGauche, pinDroit)
{
  m_stepTime = ( stepTime <  50 ?  50 : stepTime);
  m_stepUp   = ( stepUp   > 100 ? 100 : stepUp  );
  m_stepDown = ( stepDown > 100 ? 100 : stepDown);
}


void LibMoteurS::begin(void) {
  moteur.begin();
  m_begin = true;
  m_nextTimeGestion = millis() + m_stepTime;
}


void LibMoteurS::moteurs(int vitesse) {
  this->moteurGauche(vitesse);
  this->moteurDroit(vitesse);
}


void LibMoteurS::moteurGauche(int vitesse) {
  m_vitesseGaucheCible = vitesse;
}



void LibMoteurS::moteurDroit(int vitesse) {
  m_vitesseDroiteCible = vitesse;
}


void LibMoteurS::setDirection(bool dirGauche, bool dirDroite) {
  moteur.setDirection(dirGauche, dirDroite);
}


void LibMoteurS::setDebug(bool debug) {
  moteur.setDebug(debug);
}


void LibMoteurS::gestion(void) {
  if(m_begin == false) return;

  unsigned long newTime = millis();

  if (newTime > m_nextTimeGestion) {
    m_nextTimeGestion += m_stepTime;
    if (m_vitesseGaucheCourante != m_vitesseGaucheCible) {
      m_vitesseGaucheCourante = getNewVitesse(m_vitesseGaucheCourante, m_vitesseGaucheCible);
      moteur.moteurGauche(m_vitesseGaucheCourante);
    }
    if (m_vitesseDroiteCourante != m_vitesseDroiteCible) {
      m_vitesseDroiteCourante = getNewVitesse(m_vitesseDroiteCourante, m_vitesseDroiteCible);
      moteur.moteurDroit(m_vitesseDroiteCourante);
    }

  }

  // On évite l'accumulation de traitements le cas écheant
  while (newTime > m_nextTimeGestion) m_nextTimeGestion += m_stepTime;
}


int LibMoteurS::getNewVitesse(int vitesseCourante, int vitesseCible) {

  if (vitesseCible == 0) {
    // Dans tous les cas, on est en décéllération
    return getNewVitesseDown(vitesseCourante, 0);
  }

  if (vitesseCible > 0) {
    if (vitesseCourante < 0) {
      return getNewVitesseDown(vitesseCourante, 0);
    }
    else {
      if (vitesseCourante <= vitesseCible) {
        return getNewVitesseUp(vitesseCourante, vitesseCible);
      }
      else {
        return getNewVitesseDown(vitesseCourante, vitesseCible);
      }
    }
  }

  if (vitesseCible < 0) {
    if (vitesseCourante > 0) {
      return getNewVitesseDown(vitesseCourante, 0);
    }
    else {
      if (vitesseCourante >= vitesseCible) {
        return getNewVitesseUp(vitesseCourante, vitesseCible);
      }
      else {
        return getNewVitesseDown(vitesseCourante, vitesseCible);
      }
    }
  }

  return 0;
}



int LibMoteurS::getNewVitesseUp(int vitesseCourante, int vitesseCible) {
  if (m_stepUp == 0) return vitesseCible;

  // En testant le signe de l'addition des deux vitesses, on connait le sens de marche
  if ((vitesseCourante + vitesseCible) > 0) {
    // On va en avant
    vitesseCourante += m_stepUp;
    return(vitesseCourante > vitesseCible ? vitesseCible : vitesseCourante);
  }
  if ((vitesseCourante + vitesseCible) < 0) {
    // On va en arrière
    vitesseCourante -= m_stepUp;
    return(vitesseCourante < vitesseCible ? vitesseCible : vitesseCourante);
  }

  return 0; // Sinon la vitesse cible c'est 0
}



int LibMoteurS::getNewVitesseDown(int vitesseCourante, int vitesseCible) {
  if (m_stepDown == 0) return vitesseCible;

  if ((vitesseCourante + vitesseCible) > 0) {
    // On allait en avant
    vitesseCourante -= m_stepDown;
    return(vitesseCourante < vitesseCible ? vitesseCible : vitesseCourante);
  }
  if ((vitesseCourante + vitesseCible) < 0) {
    // On allait en arrière
    vitesseCourante += m_stepDown;
    return(vitesseCourante > vitesseCible ? vitesseCible : vitesseCourante);
  }
  return 0; // Si non on est déjà au plus bas
}

