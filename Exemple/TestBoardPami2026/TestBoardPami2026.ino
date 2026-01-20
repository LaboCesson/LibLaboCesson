
#include "arduino.h"
#include "LibPami2026.h"
#include "dbgArduino.h"

#define PAMI_2026_BASIC
// #define PAMI_2026_NINJA

// #define ALT_TEST

// #define TEST_AFFICHEUR
// #define TEST_RADIO
// #define TEST_CHRONO
#define TEST_GYRO
// #define TEST_MOTEUR
// #define TEST_GPIO
// #define TEST_SERVO
// #define TEST_JUMPERS

#define GPIO_SERVO PAMI_GPIO_1

#ifdef PAMI_2026_BASIC
  LibPami2026Basic pami;
#endif
#ifdef PAMI_2026_NINJA
  LibPami2026Ninja pami;
#endif


void setup() {
  Serial.begin(115200);
  delay(2000);

  #ifdef ALT_TEST
    altSetup(); return; ///////////////////
  #endif

  pami.gpio.setDebug(true);

  #ifdef TEST_AFFICHEUR
    pami.afficheur.begin();
    pami.afficheur.displayString("init");
    delay(2000);
  #endif

  #ifdef TEST_GPIO
    pami.gpio.configure(PAMI_GPIO_1, PAMI_GPIO_OUTPUT, 1);
    pami.gpio.configure(PAMI_GPIO_2, PAMI_GPIO_INPUT);
  #endif

  #ifdef TEST_SERVO
    pami.gpio.configure(GPIO_SERVO,  PAMI_GPIO_PWM, 0);
  #endif

  #ifdef TEST_CHRONO
    pami.chrono.begin();
  #endif

  #ifdef TEST_GYRO
    pami.gyro.begin();
    pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  #endif

  #ifdef TEST_RADIO
    pami.radio.begin(16);
  #endif

  #ifdef TEST_AFFICHEUR
    pami.afficheur.displayString("done");
  #endif
  delay(2000);

  #ifdef TEST_CHRONO
    #ifdef TEST_AFFICHEUR
      pami.chrono.display(true);
    #endif
    setChrono(false);
  #endif

  pami.moteur.setDebug(true);
}


#define PERIOD_AFFICHAGE    500
#define PERIOD_TEST_MOTEUR 1000
#define PERIOD_TEST_SERVO  1000

void loop(void) {

  #ifdef ALT_TEST
    altLoop(); return; ///////////////////
  #endif

  static unsigned long timeNextDisplay = millis();
         unsigned long time = millis();

  pami.gestion();

  if( time < timeNextDisplay ) return;
  timeNextDisplay += PERIOD_AFFICHAGE;

  // // Test des Jumpers
  #ifdef TEST_JUMPERS
    Serial.print(F("# Jumpers   #"));
    Serial.print(F(" Team=")); Serial.print(pami.jumper.getTeam());
    Serial.print(F(" Pami=")); Serial.println(pami.jumper.getPami());
  #endif

  // Test GPIO
  #ifdef TEST_GPIO
    static unsigned char statusLed   = LOW;
    statusLed = (statusLed == LOW ? HIGH : LOW);
    pami.gpio.set(PAMI_GPIO_1,statusLed);
    pami.gpio.set(PAMI_GPIO_2,(statusLed == LOW ? HIGH : LOW));
    Serial.print(F("# GPIO      #")); Serial.print(F(" GPIO2=")); Serial.println(pami.gpio.get(PAMI_GPIO_2));
  #endif

  #ifdef TEST_SERVO
    static unsigned long  timeNextServo = millis();
    static unsigned short statusServo = 45;
    if( time >= timeNextServo ) {
      statusServo = (statusServo == 45 ? 0 : 45);
      pami.gpio.set(GPIO_SERVO,statusServo);
      while( time >= timeNextServo ) timeNextServo += PERIOD_TEST_SERVO;
    }
  #endif

  // // Test de l'Ultrason
  // Serial.print(F("# Ultrason  #")); Serial.print(F(" Distance=")); Serial.println(pami.ultrason.getDistance());

  // Test Moteur
  #ifdef TEST_MOTEUR
    static unsigned long timeNextMoteur = millis();
    static unsigned int statusMoteur;
    if( time >= timeNextMoteur ) {
           if( statusMoteur == 0 ) {  pami.moteur.moteurGauche(100); pami.moteur.moteurDroit(0);    statusMoteur = 1; }
      else if( statusMoteur == 1 ) {  pami.moteur.moteurGauche(0);   pami.moteur.moteurDroit(-100); statusMoteur = 2; }
      else if( statusMoteur == 2 ) {  pami.moteur.moteurs(50);                                      statusMoteur = 3; }
      else if( statusMoteur == 3 ) {  pami.moteur.moteurs(0);                                       statusMoteur = 0; }
      timeNextMoteur += PERIOD_TEST_MOTEUR;
    }
  #endif

  // Test Chrono
  #ifdef TEST_CHRONO
    if( pami.chrono.isAlarm() == true ) {
      #ifdef TEST_AFFICHEUR
        pami.afficheur.setBlinking(true);
      #endif
      pami.chrono.resetAlarm();
      #ifdef TEST_GYRO
        pami.gyro.calibrate();
      #endif
      timeNextDisplay = millis();
    }
  #endif

  // Test Gyroscope
  #ifdef TEST_GYRO
    Serial.print(F("# Gyroscope #"));
    Serial.print(F("  Axe X=")); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_X));
    Serial.print(F(", Axe Y=")); Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));
    Serial.print(F(", Axe Z=")); Serial.println(pami.gyro.getAngle(GYROSCOPE_AXIS_Z));
  #endif

  //Test 433Mhz
  #ifdef TEST_RADIO
    #define RADIO_MSG_SIZE 40
    char msg[RADIO_MSG_SIZE];
    unsigned char len = pami.radio.getMessage(msg, RADIO_MSG_SIZE);
    Serial.print(F("# RADIO 433 #"));
    if( len!= 0 ) {
      Serial.print(F(" réception d'un message : '")); Serial.print(msg); Serial.println(F("'============"));
      #ifdef TEST_CHRONO
        setChrono(true);
      #endif
    } else {
      Serial.println(F(" aucun message"));
    }
  #endif

  // Serial.print(F("freeMemory="));
  // Serial.println(freeMemory());
}


void setChrono(bool status ) { // true : compteur, false : decompteur
  #ifdef TEST_AFFICHEUR
    pami.afficheur.setBlinking(false);
  #endif
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



void altLoop(void) {
}
