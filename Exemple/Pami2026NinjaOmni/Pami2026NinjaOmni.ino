/*
* Exemple de gestion d'un robot avec des roues omnidirectionnelles
* Le test peut si faire 
*   soit via la librairie LibPami2026NinjaOmni 
*   soit directement via la librairie LibMoteurOmni
*  selon l'état commenté ou non de la ligne : #define TEST_AVEC_CLASSE_PAMI 
*/

// #define TEST_AVEC_CLASSE_PAMI

#ifdef TEST_AVEC_CLASSE_PAMI

  #include "LibPami2026.h"

  LibPami2026NinjaOmni pami;

#else

  #include "LibMoteurOmni.h"

  #define PAMI_MOTEUR_AVANT_IN1 5
  #define PAMI_MOTEUR_AVANT_IN2 6
  #define PAMI_MOTEUR_AVANT_IN3 7
  #define PAMI_MOTEUR_AVANT_IN4 8

  #define PAMI_MOTEUR_ARRIERE_IN1 39
  #define PAMI_MOTEUR_ARRIERE_IN2 38
  #define PAMI_MOTEUR_ARRIERE_IN3 37
  #define PAMI_MOTEUR_ARRIERE_IN4 36

  LibMoteurOmni moteur(
    PAMI_MOTEUR_AVANT_IN1,
    PAMI_MOTEUR_AVANT_IN2,
    PAMI_MOTEUR_AVANT_IN3,
    PAMI_MOTEUR_AVANT_IN4,
    PAMI_MOTEUR_ARRIERE_IN1,
    PAMI_MOTEUR_ARRIERE_IN2,
    PAMI_MOTEUR_ARRIERE_IN3,
    PAMI_MOTEUR_ARRIERE_IN4
  );
#endif


void setup(void){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test moteur omnidirectionnelle");

  int vitesse = 50;
  int delaiAvance = 1000;
  int delaiStop   = 1000;

  #ifdef TEST_AVEC_CLASSE_PAMI

    pami.moteur.avance(DEGRE_0,  vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(DEGRE_180,vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(DEGRE_90, vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(DEGRE_270,vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(DEGRE_135,vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(DEGRE_315,vitesse);     delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(TOURNE_GAUCHE,vitesse); delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);
    pami.moteur.avance(TOURNE_DROITE,vitesse); delay(delaiAvance); pami.moteur.stop(); delay(delaiStop);

  #else

    moteur.avance(DEGRE_0,  vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(DEGRE_180,vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(DEGRE_90, vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(DEGRE_270,vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(DEGRE_135,vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(DEGRE_315,vitesse);     delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(TOURNE_GAUCHE,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);
    moteur.avance(TOURNE_DROITE,vitesse); delay(delaiAvance); moteur.stop(); delay(delaiStop);

  #endif
}


void loop(void){
}
