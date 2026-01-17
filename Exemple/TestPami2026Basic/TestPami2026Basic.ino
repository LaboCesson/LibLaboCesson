
#include "arduino.h"
#include "LibPami2026.h"

#define DUREE_WAIT_TO_RUN_PAMI  5000 // Durée d'attente avant de partir pour le PAMI (en ms)
#define DUREE_RUN_PAMI          3000 // Durée de mouvement pour le PAMI (en ms)

LibPami2026Basic pami;

// Gestion des servomoteurs
#define GPIO_BOULE_FACETTE PAMI_GPIO_1
#define DEFAULT_FACETTE_ANGLE 90    // L'angle au repos en degré de la boule à facette
#define MAX_BRAS_BOULE        45    // Angle maximum de rotation de la boule
#define PERIOD_GESTION_BOULE  500   // Période d'oscillation de la boule en ms

bool flagActivity = false; // Indique si l'activité Bras ou Boule doit être activée

// Gestion de la radio
typedef enum {
  RADIO_NO_ORDER = 0, ///< Aucun message de reçu
  RADIO_START    = 1, ///< Message de départ
  RADIO_STOP     = 2, ///< Message d'arrêt
} t_radioOrder;

// Gestion des moteurs
#define VITESSE_MOTEUR 60
bool runPami = false;

// Gestion de la direction du PAMI
#define PERIOD_GESTION_DIRECTION  100 // Période d'évaluation de la direction en ms
#define TIME_BEFORE_RECALIBRAGE  2000 // Temps d'attente avant recalibrage en ms
bool recalibrationDone = false;       // Permet de recalibrer le gyroscope pendant la période d'attente

// Gestion des états du PAMI
// Chaque état correspond à une étape du PAMI
typedef enum {
  PAMI_SAY_READY    = 0, // Le PAMI dit qu'il est en attente d'un ordre => 1 seconde d'activité
  PAMI_WAIT_START   = 1, // Le PAMI est en attente d'un ordre de départ 
  PAMI_SAY_OK_START = 2, // Le PAMI dit qu'il a recu l'odre de départ   =>  3 secondes d'activité
  PAMI_WAIT_TO_RUN  = 3, // Le PAMI attends pour partir
  PAMI_ON_ROAD      = 4, // Le PAMI est en route
  PAMI_ARRIVED      = 5  // Le PAMI est arrivé => Il applaudit ou fait tourner la boule
} t_statusPami;
t_statusPami statusPami = PAMI_SAY_READY;

#define DUREE_SAY_READY    1000 // Durée de battement des bras pour acquitter son initialisation (en ms)
#define DUREE_SAY_OK_START 1000 // Durée de battement des bras pour acquitter acquitter la réception radio (en ms)

unsigned long dureeWaitToRun; // Temps d'attente avant de démarrer le parcours
unsigned long dureeRunPami;   // Temps de durée du parcours
unsigned long endStatusTime;  // Fin d'un état en cours avant passage vers le suivant


void setup(void){
  Serial.begin(115200);
  delay(2000);

  Serial.println("PAMI controlé par Radio");

  // pami.gpio.setDebug(true);
  // pami.radio.setDebug(true);

  // On initialise les différents temps
  dureeWaitToRun = DUREE_WAIT_TO_RUN_PAMI;
  dureeRunPami   = DUREE_RUN_PAMI;

  Serial.print("PAMI Team = "); Serial.println( pami.jumper.getTeam() == PAMI_TEAM_A? "A":"B");
  Serial.print("PAMI N° = ");   Serial.println( pami.jumper.getPami() );

  // On configure les moteur
  pami.moteur.begin();
  // pami.moteur.setDebug(true);
  pami.moteur.moteurs(0);
  pami.moteur.setDirection(false,true);

  // On configure la radio
  pami.radio.begin(16);

  // On configure les GPIO
  pami.gpio.configure(GPIO_BOULE_FACETTE, PAMI_GPIO_PWM, DEFAULT_FACETTE_ANGLE);

  // on signale le démarrage
  pami.gpio.set(GPIO_BOULE_FACETTE, DEFAULT_FACETTE_ANGLE+MAX_BRAS_BOULE);
  delay(500);
  pami.gpio.set(GPIO_BOULE_FACETTE, DEFAULT_FACETTE_ANGLE);

  // On initialise le gyroscope
  pami.gyro.begin();
  pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  pami.gyro.display(true);

  // On lance le PAMI vers sa première étape
  flagActivity = true;
  recalibrationDone = false;
  switchToSayReady();
}


void loop(void){

   displayStatus(); // On affiche le Status sur la console pour le debug

  // En fonction du status, on appelle la fonction correspondant à l'étape
  switch( statusPami ) {
    case PAMI_SAY_READY    : pamiSayReady();   break;
    case PAMI_WAIT_START   : pamiWaitStart();  break; 
    case PAMI_SAY_OK_START : pamiSayOkStart(); break; 
    case PAMI_WAIT_TO_RUN  : pamiWaitToRun();  break; 
    case PAMI_ON_ROAD      : pamiOnRoad();     break; 
    case PAMI_ARRIVED      : pamiArrived();    break; 
  }

  gestionBoule(); // Gestion de la boule
  gestionRun();   // Gestion du run du PAMI
  gestionRadio(); // Gestion des réceptions radio
  pami.gestion(); // Gestion des organes du PAMI
}


// Les routines suivantes sont appelées pour passer à une autre étape
void switchToSayReady()   { statusPami = PAMI_SAY_READY;    endStatusTime = millis()+DUREE_SAY_READY;    }
void switchToWaitStart()  { statusPami = PAMI_WAIT_START;                                                }
void switchToSayOkStart() { statusPami = PAMI_SAY_OK_START; endStatusTime = millis()+DUREE_SAY_OK_START; }
void switchToWaitToRun()  { statusPami = PAMI_WAIT_TO_RUN;  endStatusTime = millis()+dureeWaitToRun;     }
void switchToOnRoad()     { statusPami = PAMI_ON_ROAD;      endStatusTime = millis()+dureeRunPami;       }
void switchToArrived()    { statusPami = PAMI_ARRIVED;                                                   }


// Dans cette étape, le PAMI indique qu'il est prêt à recevoir un ordre de départ
void pamiSayReady(void) {
  if( endStatusTime > millis()) return; 
  flagActivity = false;
  switchToWaitStart();
}

// Dans cette étape, le PAMI attend un ordre radio de départ
void pamiWaitStart(void) {
  if( gestionRadio() != RADIO_START ) return;
  flagActivity = true;
  switchToSayOkStart();
}


// Dans cette étape, le PAMI indique qu'il a bien reçu l'ordre de départ
void pamiSayOkStart(void) {
  if( endStatusTime > millis()) return;
  flagActivity = false;
  switchToWaitToRun();
}


// Dans cette étape, le PAMI attends la fin de la manche pour partir
void pamiWaitToRun() {
  if( recalibrationDone == false ) {
    pami.gyro.calibrate();
    recalibrationDone = true; 
  }

  // // Possiblité d'interrompre le PAMI et d'attendre de nouveau un ordre de départ
  // if( gestionRadio() == RADIO_STOP ) {
  //   switchToWaitStart();
  // }

  if( endStatusTime > millis()) return;
  // pami.moteur.moteurs(VITESSE_MOTEUR);
  // pami.moteur.moteurGauche(VITESSE_MOTEUR);
  // pami.moteur.moteurDroit(VITESSE_MOTEUR+20);
  runPami = true;
  switchToOnRoad();
}


// Dans cette étape, le PAMI est en route vers sa destination
void pamiOnRoad() {
  if( endStatusTime > millis()) return;
  pami.moteur.moteurs(0);
  runPami = false;
  flagActivity = true;
  switchToArrived();
}


// Dans cette étape, le PAMI est arrivé et applaudit
void pamiArrived() {
  // // Possiblité d'interrompre le clap
  // // UNIQUEMENT POUR LE DEBUG
  // if( gestionRadio() == RADIO_STOP ) {
  //   switchToWaitStart();
  // }
}


// Cette routine permet de tester si un ordre radio est arrivé
t_radioOrder gestionRadio( void ) {
  #define RADIO_MSG_SIZE 40
  char msg[RADIO_MSG_SIZE];

  unsigned char len = pami.radio.getMessage(msg, RADIO_MSG_SIZE);

  if( len == 0 )  return RADIO_NO_ORDER;
  if( msg[0] != 'T' ) return RADIO_NO_ORDER;
  if( msg[1] != 'A' ) return RADIO_NO_ORDER;
  if( msg[2] == 'G' ) return RADIO_START;
  if( msg[2] == 'S' ) return RADIO_STOP;
  return RADIO_NO_ORDER;
}


// Cette routine permet de gérer la boule
void gestionBoule( void ) {
  static short lastValue = 0;
  static unsigned long nextTime = millis()+PERIOD_GESTION_BOULE;
  static bool resetLedsDone = false;

  // On ne gére la boule que toutes les PERIOD_GESTION_BOULE ms
  if( millis() < nextTime) return;
  while( nextTime <= millis() ) nextTime += PERIOD_GESTION_BOULE;

  if( !flagActivity ) { return; }

  if( lastValue == 0 ) {
    pami.gpio.set(GPIO_BOULE_FACETTE, DEFAULT_FACETTE_ANGLE+MAX_BRAS_BOULE);
    lastValue = 1;
  } else {
    pami.gpio.set(GPIO_BOULE_FACETTE, DEFAULT_FACETTE_ANGLE-MAX_BRAS_BOULE);
    lastValue = 0;
  }
}


// Cette routine permet de gérer le run du PAMI
void gestionRun() {
  static unsigned long  nextTime = millis()+PERIOD_GESTION_DIRECTION;
  int anglePami;
  char vitesseGauche = VITESSE_MOTEUR;
  char vitesseDroite = VITESSE_MOTEUR;

  // On ne gére la direction que toutes les PERIOD_GESTION_DIRECTION ms
  if( millis() < nextTime) return;
  while( nextTime <= millis() ) nextTime += PERIOD_GESTION_DIRECTION;

  if( !runPami ) return; 
  anglePami = pami.gyro.getAngle();

  if( anglePami < 0 ) {
    vitesseGauche += 10;
  }
  if( anglePami > 0 ) {
    vitesseDroite += 10;
  }

  pami.moteur.moteurGauche(vitesseGauche);
  pami.moteur.moteurDroit(vitesseDroite);
}


// Cette routine permet d'afficher sur la console et pour le debug, l'étape en cours
void displayStatus( void ) {
  static t_statusPami lastDisplayStatus = (t_statusPami) 100;

  if( statusPami == lastDisplayStatus ) return;

  switch( statusPami ) {
    case PAMI_SAY_READY    : Serial.println("PAMI_SAY_READY");    break;
    case PAMI_WAIT_START   : Serial.println("PAMI_WAIT_START");   break; 
    case PAMI_SAY_OK_START : Serial.println("PAMI_SAY_OK_START"); break; 
    case PAMI_WAIT_TO_RUN  : Serial.println("PAMI_WAIT_TO_RUN");  break; 
    case PAMI_ON_ROAD      : Serial.println("PAMI_ON_ROAD");      break; 
    case PAMI_ARRIVED      : Serial.println("PAMI_ARRIVED");      break; 
  }
  lastDisplayStatus = statusPami;
}  

