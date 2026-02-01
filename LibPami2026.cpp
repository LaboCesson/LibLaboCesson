/*!
 * \file 	LibPami2026.cpp
 * \brief 	Le fichier source des classes de gestion des PAMIs 2026 
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#include "LibPami2026.h"

// Définition des jumpers associant une carte à un PAMI
#define PAMI_JUMPER_TEAM  PAMI_2026_JUMPER_PIN1
#define PAMI_JUMPER_NB_L  PAMI_2026_JUMPER_PIN2
#define PAMI_JUMPER_NB_M  PAMI_2026_JUMPER_PIN3
#define PAMI_JUMPER_NB_H  PAMI_2026_JUMPER_PIN4

unsigned char jumpersPinList[] = { PAMI_JUMPER_NB_H,PAMI_JUMPER_NB_M,PAMI_JUMPER_NB_L };

// Définition des pins de controle d'un couple de moteurs
#define PAMI_MOTEURS1_IN1 5
#define PAMI_MOTEURS1_IN2 6
#define PAMI_MOTEURS1_IN3 7
#define PAMI_MOTEURS1_IN4 8

// Définition des pins de controle de deux couples de moteurs associés à des roues omnidirectionnelles
#define PAMI_MOTEUR_AVANT_IN1 PAMI_MOTEURS1_IN1
#define PAMI_MOTEUR_AVANT_IN2 PAMI_MOTEURS1_IN2
#define PAMI_MOTEUR_AVANT_IN3 PAMI_MOTEURS1_IN3
#define PAMI_MOTEUR_AVANT_IN4 PAMI_MOTEURS1_IN4

#define PAMI_MOTEUR_ARRIERE_IN1 39
#define PAMI_MOTEUR_ARRIERE_IN2 38
#define PAMI_MOTEUR_ARRIERE_IN3 37
#define PAMI_MOTEUR_ARRIERE_IN4 36

// Définition des pins associées aux GPIO
#define PAMI_PIN_GPIO_1  9
#define PAMI_PIN_GPIO_2 10
#define PAMI_PIN_GPIO_3 11
#define PAMI_PIN_GPIO_4 12
#define PAMI_PIN_GPIO_5 13
#define PAMI_PIN_GPIO_6 14

// Définition des pins associées aux LEDs
#define PAMI_PIN_LED_BLEU  17
#define PAMI_PIN_LED_JAUNE 18
#define PAMI_PIN_LED_VERTE 21

// Définition du pin associée au switch Coté Jaune/Bleu
#define PAMI_PIN_COTE_JAUNE_BLEU  34


// Définition des GPIO des PAMIs Basique (les GPIO 1 et 2 sont associés aux servomoteur 360°)
unsigned char gpioPinListBasic[] = {PAMI_PIN_GPIO_3,PAMI_PIN_GPIO_4,PAMI_PIN_GPIO_5,PAMI_PIN_GPIO_6};
unsigned char nbGpioPinBasic = 4;

// Définition des GPIO des PAMIs Ninja
unsigned char gpioPinListNinja[] = {PAMI_PIN_GPIO_1,PAMI_PIN_GPIO_2,PAMI_PIN_GPIO_3,PAMI_PIN_GPIO_4,PAMI_PIN_GPIO_5,PAMI_PIN_GPIO_6};
unsigned char nbGpioPinNinja = 6;


// Définition des pins associées aà l'afficheur 7 segments
#define PAMI_AFF1637_DATA 15
#define PAMI_AFF1637_CLK  16

// Définition des pins associées au récepteur Radio
#define PAMI_433MHZ_PIN     40
#define PAMI_433MHZ_SPEED   2000
#define PAMI_433MHZ_PATTERN "LaboCesson"

//
// Gestion d'un PAMI BASIC
//

LibPami2026Basic::LibPami2026Basic() :
  afficheur(PAMI_AFF1637_CLK,PAMI_AFF1637_DATA),
  jumper(PAMI_JUMPER_TEAM, jumpersPinList, 3),
  moteur(PAMI_PIN_GPIO_1, PAMI_PIN_GPIO_2,50,10,0),
  gpio(gpioPinListBasic,nbGpioPinBasic),
  gyro(),
  radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
  chrono()
{
  // LibChrono::setDisplay(dynamic_cast<LibAff1637*>(this));
  gyro.setDisplay(&afficheur);
  chrono.setDisplay(&afficheur);

  // Configuration des Pins associées aux LEDs et au switch Coté Jaune/Bleu
  pinMode(PAMI_PIN_LED_BLEU,  OUTPUT);
  pinMode(PAMI_PIN_LED_JAUNE, OUTPUT);
  pinMode(PAMI_PIN_LED_VERTE, OUTPUT);
  pinMode(PAMI_PIN_COTE_JAUNE_BLEU, INPUT_PULLUP);

  // Par défaut le PAMI est sur le coté Bleu
  m_cote_plateau = PAMI_COTE_BLEU;
  digitalWrite(PAMI_PIN_LED_BLEU,  LOW);
  digitalWrite(PAMI_PIN_LED_JAUNE, LOW);

}


void LibPami2026Basic::gestion(void) {
  afficheur.gestion();
  gyro.gestion();
  chrono.gestion();
  moteur.gestion();

  // Gestion des LEDs et du switch Coté Jaune/Bleu
  t_pami_cote_plateau newSide = (digitalRead(PAMI_PIN_COTE_JAUNE_BLEU) == LOW ? PAMI_COTE_BLEU : PAMI_COTE_JAUNE);

  if ((newSide != m_cote_plateau)||(m_init_led == false)) {
    m_cote_plateau = newSide;
    m_init_led = true;
    digitalWrite(PAMI_PIN_LED_BLEU,  (m_cote_plateau == PAMI_COTE_BLEU  ? LOW : HIGH));
    digitalWrite(PAMI_PIN_LED_JAUNE, (m_cote_plateau == PAMI_COTE_JAUNE ? LOW : HIGH));
  }
}



//
// Gestion d'un PAMI NINJA avec roues normales
//

LibPami2026Ninja::LibPami2026Ninja() :
  afficheur(PAMI_AFF1637_CLK,PAMI_AFF1637_DATA),
  jumper(PAMI_JUMPER_TEAM, jumpersPinList, 3),
  moteur(PAMI_MOTEURS1_IN1, PAMI_MOTEURS1_IN2, PAMI_MOTEURS1_IN3, PAMI_MOTEURS1_IN4),
  gpio(gpioPinListNinja,nbGpioPinNinja),
  gyro(),
  radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
  chrono()
{
  gyro.setDisplay(&afficheur);
  chrono.setDisplay(&afficheur);
}


void LibPami2026Ninja::gestion(void) {
  afficheur.gestion();
  gyro.gestion();
  chrono.gestion();
}


//
// Gestion d'un PAMI NINJA avec roues omnidirectionnelles
//

LibPami2026NinjaOmni::LibPami2026NinjaOmni() :
  afficheur(PAMI_AFF1637_CLK, PAMI_AFF1637_DATA),
  //jumper(PAMI_JUMPER_TEAM, jumpersPinList, 3),
  moteur(PAMI_MOTEUR_AVANT_IN1,  PAMI_MOTEUR_AVANT_IN2,  PAMI_MOTEUR_AVANT_IN3,  PAMI_MOTEUR_AVANT_IN4,
         PAMI_MOTEUR_ARRIERE_IN1,PAMI_MOTEUR_ARRIERE_IN2,PAMI_MOTEUR_ARRIERE_IN3,PAMI_MOTEUR_ARRIERE_IN4),
  gpio(gpioPinListNinja, nbGpioPinNinja),
  gyro(),
  radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
  telemetre(),
  chrono()
{
  gyro.setDisplay(&afficheur);
  chrono.setDisplay(&afficheur);
}


void LibPami2026NinjaOmni::begin(void) {
  pinMode(PAMI_OMNI_SWITCH_AVANT,   INPUT_PULLUP);
  pinMode(PAMI_OMNI_SWITCH_LATERAL, INPUT_PULLUP);
  pinMode(PAMI_OMNI_SWITCH_ARRIERE, INPUT_PULLUP);

  gpio.configure(PAMI_GPIO_BRAS_AVANT, PAMI_GPIO_PWM, PAMI_OMNI_BRAS_AVANT_FERME);
  gpio.configure(PAMI_GPIO_BRAS_ARRIERE, PAMI_GPIO_PWM, PAMI_OMNI_BRAS_ARRIERE_FERME);

  gyro.begin();
  gyro.selectAxis(GYROSCOPE_AXIS_X);

  telemetre.begin();
}


void LibPami2026NinjaOmni::gestion(void) {
  afficheur.gestion();
  gyro.gestion();
  chrono.gestion();
}
