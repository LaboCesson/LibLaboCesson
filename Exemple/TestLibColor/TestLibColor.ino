/*
* Exemple d'utilisation des librairies LibTcs3472 et LibMultiTcs3472
*
* Le choix de la librairie à tester dépend de la ligne : #define TEST_LIB_MULTI_TCS3472
* Si commentée => Test LibTcs3472
* Si non commentée => Test LibMultiTcs3472
*/

#include "LibColor.h"

#define TEST_LIB_MULTI_TCS3472

#ifdef TEST_LIB_MULTI_TCS3472

  LibMultiTcs3472 colors;

  #define NB_COLORS 4

  unsigned char muxInput[NB_COLORS] = {0,1,2,3};

  void setup(void){
    Serial.begin(115400);
    Wire.begin();
    delay(2000);
    Serial.println("Test de la librairie LibMultiTcs3472");
    colors.begin(NB_COLORS,muxInput);
  }

  void loop(void){
    t_robot_color cols[NB_COLORS];

    colors.gestion();
    colors.getColors(NB_COLORS,cols);
    Serial.print("Couleurs : ");
    for(int i=0;i<NB_COLORS;i++) {
      switch(cols[i]) {
        case ROBOT_COULEUR_INCONNUE : Serial.print("???? ");  break;
        case ROBOT_COULEUR_JAUNE    : Serial.print("JAUNE "); break;
        case ROBOT_COULEUR_BLEU     : Serial.print("BLEU ");  break;
      }
    }
    Serial.println("");
    delay(1000);
  }

#else

  LibTcs3472      color;

  void setup(void){
    Serial.begin(115400);
    Wire.begin();
    delay(2000);
    Serial.println("Test de la librairie LibTcs3472");
    color.begin();
  }

  void loop(void){
    t_robot_color col;

    color.gestion();
    col = color.getColor();
      switch(col) {
        case ROBOT_COULEUR_INCONNUE : Serial.println("???? ");  break;
        case ROBOT_COULEUR_JAUNE    : Serial.println("JAUNE "); break;
        case ROBOT_COULEUR_BLEU     : Serial.println("BLEU ");  break;
      }
    delay(1000);
  }

#endif


