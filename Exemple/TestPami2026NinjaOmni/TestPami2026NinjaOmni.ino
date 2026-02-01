/*
* Exemple de gestion d'un robot avec des roues omnidirectionnelles
*/

#include "LibPami2026.h"

LibPami2026NinjaOmni pami;

// #define TEST_GYRO
// #define TEST_TELEMETRE

void setup(void){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test PAMI avec moteur omnidirectionnelle");

  pami.begin();

  #ifdef TEST_GYRO
    pami.gyro.display(true);
  #endif
}


#define VITESSE_PAMI_TEST        50
#define DELAI_AVANCE_PAMI_TEST 1000
#define DELAI_STOP_PAMI_TEST   1000

void loop(void){

  pami.gestion();

  if(digitalRead(PAMI_OMNI_SWITCH_AVANT) == LOW) {
    pami.gpio.set(PAMI_GPIO_BRAS_AVANT,  PAMI_OMNI_BRAS_AVANT_OUVERT  );
  } else {
    pami.gpio.set(PAMI_GPIO_BRAS_AVANT,  PAMI_OMNI_BRAS_AVANT_FERME   );
  }

  if(digitalRead(PAMI_OMNI_SWITCH_ARRIERE) == LOW) {
    pami.gpio.set(PAMI_GPIO_BRAS_ARRIERE,  PAMI_OMNI_BRAS_ARRIERE_OUVERT  );
  } else {
    pami.gpio.set(PAMI_GPIO_BRAS_ARRIERE,  PAMI_OMNI_BRAS_ARRIERE_FERME   );
  }

  if( digitalRead(PAMI_OMNI_SWITCH_LATERAL) == LOW ) {
    pami.moteur.avance(DEGRE_0,  VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(DEGRE_180,VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(DEGRE_90, VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(DEGRE_270,VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(DEGRE_135,VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(DEGRE_315,VITESSE_PAMI_TEST);     delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(TOURNE_GAUCHE,VITESSE_PAMI_TEST); delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
    pami.moteur.avance(TOURNE_DROITE,VITESSE_PAMI_TEST); delay(DELAI_AVANCE_PAMI_TEST); pami.moteur.stop(); delay(DELAI_STOP_PAMI_TEST);
  }

  if( pami.telemetre.getDistance() < 150 ) {
    pami.moteur.avance(DEGRE_270,VITESSE_PAMI_TEST);
  } else {
    pami.moteur.stop();
  }

  #ifdef TEST_GYRO
    Serial.println(pami.gyro.getAngle());
    delay(500);
  #endif

  #ifdef TEST_TELEMETRE
    Serial.print("Distance="); Serial.println(pami.telemetre.getDistance());
    delay(500);
  #endif

  return;
}
