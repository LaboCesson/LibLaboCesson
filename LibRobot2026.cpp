/*!
 * \file 	  LibPRobot2026.cpp
 * \brief 	Le fichier source des classes de gestion des robots 2026 
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2026
 */

#include "LibRobot2026.h"

// Definition des pins associés au BUS CAN
#define ROBOT_RECV_CAN_BUS_SCK_PIN  1
#define ROBOT_RECV_CAN_BUS_MOSI_PIN 2
#define ROBOT_RECV_CAN_BUS_MISO_PIN 3
#define ROBOT_RECV_CAN_BUS_CS_PIN   4

#define ROBOT_SEND_CAN_BUS_CS_PIN  49

// Definition des pins de controle du couple de moteur 1
#define ROBOT_MOTEUR1_IN1 5
#define ROBOT_MOTEUR1_IN2 6
#define ROBOT_MOTEUR1_IN3 7
#define ROBOT_MOTEUR1_IN4 8

// Definition des pins de controle du couple de moteur 2
#define ROBOT_MOTEUR2_IN1 39
#define ROBOT_MOTEUR2_IN2 38
#define ROBOT_MOTEUR2_IN3 37
#define ROBOT_MOTEUR2_IN4 36

// Definition des pins associées aux PWM
#define PIN_GPIO_1  9
#define PIN_GPIO_2 10
#define PIN_GPIO_3 11
#define PIN_GPIO_4 12
#define PIN_GPIO_5 13
#define PIN_GPIO_6 14

unsigned char gpioPinListRobot1[] = { PIN_GPIO_1,PIN_GPIO_2,PIN_GPIO_3,PIN_GPIO_4,PIN_GPIO_5,PIN_GPIO_6 };
unsigned char nbGpioPinRobot1 = 6;


//=====================================
// Class LibRobot2026Robot1Recv
//=====================================

LibRobot2026Robot1Recv::LibRobot2026Robot1Recv() :
  canBus(ROBOT_RECV_CAN_BUS_SCK_PIN, ROBOT_RECV_CAN_BUS_MISO_PIN, ROBOT_RECV_CAN_BUS_MOSI_PIN, ROBOT_RECV_CAN_BUS_CS_PIN),
  canProt(),
  moteur1(ROBOT_MOTEUR1_IN1, ROBOT_MOTEUR1_IN2, ROBOT_MOTEUR1_IN3, ROBOT_MOTEUR1_IN4),
  moteur2(ROBOT_MOTEUR2_IN1, ROBOT_MOTEUR2_IN2, ROBOT_MOTEUR2_IN3, ROBOT_MOTEUR2_IN4),
  //color()
  //chrono()
  // afficheur(PAMI_AFF1637_CLK,PAMI_AFF1637_DATA),
  // jumper(PAMI_JUMPER_TEAM, jumpersPinList, 3),
   gpio(gpioPinListRobot1,nbGpioPinRobot1)
  // radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
{
  #ifdef ARDUINO_ARCH_ESP32
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
  #endif

  canProt.setCanBusDriver(&canBus);
  canProt.setMoteurDriver(0,&moteur1);
  canProt.setMoteurDriver(1,&moteur2);
  canProt.setGpioDriver(&gpio);
  //canProt.setColorDriver(&color);

  canProt.begin();
  moteur1.begin();
  moteur2.begin();
  // color.begin();


  // chrono.setDisplay(&afficheur);

  // Configuration des pins et sorties PWM
  gpio.configure(3, PAMI_GPIO_OUTPUT_A);
  gpio.configure(0, PAMI_GPIO_PWM);

}


void LibRobot2026Robot1Recv::gestion(void) {
  canProt.gestionMessage();
  //color.gestion();
  //chrono.gestion();
  // afficheur.gestion();
}


//=====================================
// Class LibRobot2026Robot1Send
//=====================================

LibRobot2026Robot1Send::LibRobot2026Robot1Send() :
  canBus(ROBOT_SEND_CAN_BUS_CS_PIN),
  canProt(),
  chrono()

  // afficheur(PAMI_AFF1637_CLK,PAMI_AFF1637_DATA),
  // jumper(PAMI_JUMPER_TEAM, jumpersPinList, 3),
  // gpio(gpioPinListBasic,nbGpioPinBasic),
  // radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
{
  canProt.setCanBusDriver(&canBus);

  // chrono.setDisplay(&afficheur);
}


void LibRobot2026Robot1Send::gestion(void) {
  // afficheur.gestion();
  chrono.gestion();
}
