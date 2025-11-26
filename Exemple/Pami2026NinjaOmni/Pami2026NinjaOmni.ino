#include "LibMoteurOmni.h"

LibMoteurOmni moteur;

void setup(void){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test moteur omnidirectionnelle");

  // moteur.moteur1.setDebug(true);
  int vitesse = 50;
  int delaiAvance = 1000;
  int delaiStop   = 1000;

  moteur.avance(DEGRE_0,  vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(DEGRE_180,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(DEGRE_90, vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(DEGRE_270,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(DEGRE_135,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(DEGRE_315,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(TOURNE_GAUCHE,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
  moteur.avance(TOURNE_DROITE,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
}


void loop(void){
}
