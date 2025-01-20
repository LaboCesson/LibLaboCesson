
#include "arduino.h"
#include "LibPami.h"
#include "MemoryFree.h"

LibPami pami;


void setup() {
  Serial.begin(9600);

  Serial.print("freeMemory=");
  Serial.println(freeMemory());

  pami.afficheur.begin();
  pami.afficheur.displayString("init");
  pami.ultrason.begin();
  pami.ultrason.setMaxDistance(500);
  pami.gpio.configure(PAMI_GPIO_1, PAMI_GPIO_OUTPUT, 1);
  pami.gpio.configure(PAMI_GPIO_2, PAMI_GPIO_INPUT);
  pami.gpio.configure(PAMI_GPIO_3, PAMI_GPIO_PWM, 0);
  pami.chrono.begin();
  pami.gyro.begin();
  pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  pami.radio.begin(16);

  pami.afficheur.displayString("done");
  delay(2000);

  pami.chrono.display(true);
  setChrono(false);
}


#define PERIOD_AFFICHAGE    500
#define PERIOD_TEST_MOTEUR 1000
#define PERIOD_TEST_SERVO  1000

void loop(void) {
  static unsigned long timeNextDisplay = millis();
         unsigned long time = millis();

  pami.gestion();

  if( time < timeNextDisplay ) return;
  timeNextDisplay += PERIOD_AFFICHAGE;

  // Test des Jumpers
  Serial.print("# Jumpers   #");
  Serial.print(" Team="); Serial.print(pami.jumper.getTeam());
  Serial.print(" Pami="); Serial.println(pami.jumper.getPami());

  // Test GPIO
  static unsigned char statusLed   = LOW;
  statusLed = (statusLed == LOW ? HIGH : LOW);
  pami.gpio.set(PAMI_GPIO_1,statusLed);
  Serial.print("# GPIO      #"); Serial.print(" GPIO2="); Serial.println(pami.gpio.get(PAMI_GPIO_2));

  static unsigned long  timeNextServo = millis();
  static unsigned short statusServo = 45;
  if( time >= timeNextServo ) {
    statusServo = (statusServo == 45 ? 0 : 45);
    pami.gpio.set(PAMI_GPIO_3,statusServo);
    while( time >= timeNextServo ) timeNextServo += PERIOD_TEST_SERVO;
  }

  // Test de l'Ultrason
  Serial.print("# Ultrason  #"); Serial.print(" Distance="); Serial.println(pami.ultrason.getDistance());

  // Test Moteur
  static unsigned long timeNextMoteur = millis();
  static unsigned int statusMoteur;
  if( time >= timeNextMoteur ) {
         if( statusMoteur == 0 ) {  pami.moteur.moteurs(50); statusMoteur = 1; }
    else if( statusMoteur == 1 ) {  pami.moteur.moteurs(0);  statusMoteur = 2; }
    else if( statusMoteur == 2 ) {  pami.moteur.moteurGauche(100); pami.moteur.moteurDroit(0); statusMoteur = 3; }
    else if( statusMoteur == 3 ) {  pami.moteur.moteurGauche(0);   pami.moteur.moteurDroit(100); statusMoteur = 0; }
    timeNextMoteur += PERIOD_TEST_MOTEUR;
  }

  // if( time >= timeNextMoteur ) {
  //   // pami.moteur.moteurDroit(50);
  //   Serial.print("# Moteur    #"); Serial.println(statusMoteur);
  //        if( statusMoteur == 0 ) {  pami.moteur.moteurDroit(0);   statusMoteur = 1; }
  //   else if( statusMoteur == 1 ) {  pami.moteur.moteurDroit(25);  statusMoteur = 2; }
  //   else if( statusMoteur == 2 ) {  pami.moteur.moteurDroit(50);  statusMoteur = 3; }
  //   else if( statusMoteur == 3 ) {  pami.moteur.moteurDroit(100); statusMoteur = 0; }
  //   timeNextMoteur += PERIOD_TEST_MOTEUR;
  // }


  // static int vitesse = 0;
  // vitesse += 10;
  // if( vitesse > 100 ) vitesse = 0;
  // pami.moteur.moteurGauche(vitesse);
  // Serial.print("# Moteur    #"); Serial.println(vitesse);



  // Test Chrono
  if( pami.chrono.isAlarm() == true ) {
    pami.afficheur.setBlinking(true);
    pami.chrono.resetAlarm();
    pami.gyro.calibrate();
    timeNextDisplay = millis();
  }

  // Test Gyroscope
  Serial.print("# Gyroscope #");
  Serial.print("  Axe X="); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_X));
  Serial.print(", Axe Y="); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));
  Serial.print(", Axe Z="); Serial.println(pami.gyro.getAngle(GYROSCOPE_AXIS_Z));

  //Test 433Mhz
  #define RADIO_MSG_SIZE 40
  char msg[RADIO_MSG_SIZE];
  unsigned char len = pami.radio.getMessage(msg, RADIO_MSG_SIZE);
  Serial.print("# RADIO 433 #");
  if( len!= 0 ) {
    Serial.print(" réception d'un message : '"); Serial.print(msg); Serial.println("'");
    setChrono(true);
  } else {
    Serial.println(" aucun message");
  }

  Serial.print("freeMemory=");
  Serial.println(freeMemory());
}


void setChrono(bool status ) { // true : compteur, false : decompteur
  pami.afficheur.setBlinking(false);
  if( status == true ) {
    pami.chrono.setMode(CHRONO_MODE_COMPTEUR);
    pami.chrono.setAlarme(25);
    pami.chrono.start(15);

  } else {
    pami.chrono.setMode(CHRONO_MODE_DECOMPTE);
    pami.chrono.setAlarme(5);
    pami.chrono.start(15);
  }
}


