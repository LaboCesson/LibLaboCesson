
#include "arduino.h"
#include "LibPami.h"
#include "MemoryFree.h"

LibPami pami;


void setup() {
  Serial.begin(9600);

  Serial.print(F("freeMemory="));
  Serial.println(freeMemory());

  pami.afficheur.begin();
  pami.afficheur.displayString("init");
  pami.ultrason.begin();
  pami.ultrason.setMaxDistance(500);
  pami.gpio.configure(PAMI_GPIO_1, PAMI_GPIO_OUTPUT, 1);
  pami.gpio.configure(PAMI_GPIO_2, PAMI_GPIO_OUTPUT, 1);
//  pami.gpio.configure(PAMI_GPIO_2, PAMI_GPIO_INPUT);
  pami.gpio.configure(PAMI_GPIO_3, PAMI_GPIO_PWM, 0);
  pami.chrono.begin();
  pami.gyro.begin();
  pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  pami.radio.begin(16);
  pami.moteur.setPwmMode(false);

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
  Serial.print(F("# Jumpers   #"));
  Serial.print(F(" Team=")); Serial.print(pami.jumper.getTeam());
  Serial.print(F(" Pami=")); Serial.println(pami.jumper.getPami());

  // Test GPIO
  static unsigned char statusLed   = LOW;
  statusLed = (statusLed == LOW ? HIGH : LOW);
  pami.gpio.set(PAMI_GPIO_1,statusLed);
  pami.gpio.set(PAMI_GPIO_2,(statusLed == LOW ? HIGH : LOW));
  Serial.print(F("# GPIO      #")); Serial.print(F(" GPIO2=")); Serial.println(pami.gpio.get(PAMI_GPIO_2));


  static unsigned long  timeNextServo = millis();
  static unsigned short statusServo = 45;
  if( time >= timeNextServo ) {
    statusServo = (statusServo == 45 ? 0 : 45);
    pami.gpio.set(PAMI_GPIO_3,statusServo);
    while( time >= timeNextServo ) timeNextServo += PERIOD_TEST_SERVO;
  }

  // Test de l'Ultrason
  Serial.print(F("# Ultrason  #")); Serial.print(F(" Distance=")); Serial.println(pami.ultrason.getDistance());

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

  // Test Chrono
  if( pami.chrono.isAlarm() == true ) {
    pami.afficheur.setBlinking(true);
    pami.chrono.resetAlarm();
    pami.gyro.calibrate();
    timeNextDisplay = millis();
  }

  // Test Gyroscope
  Serial.print(F("# Gyroscope #"));
  Serial.print(F("  Axe X=")); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_X));
  Serial.print(F(", Axe Y=")); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));
  Serial.print(F(", Axe Z=")); Serial.println(pami.gyro.getAngle(GYROSCOPE_AXIS_Z));

  //Test 433Mhz
  #define RADIO_MSG_SIZE 40
  char msg[RADIO_MSG_SIZE];
  unsigned char len = pami.radio.getMessage(msg, RADIO_MSG_SIZE);
  Serial.print(F("# RADIO 433 #"));
  if( len!= 0 ) {
    Serial.print(F(" réception d'un message : '")); Serial.print(msg); Serial.println(F("'============"));
    setChrono(true);
  } else {
    Serial.println(F(" aucun message"));
  }

  Serial.print(F("freeMemory="));
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


