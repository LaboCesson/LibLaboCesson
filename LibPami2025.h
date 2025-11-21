/*!
 * \file 	LibPami2025.h
 * \brief 	Le fichier de définition de la classe LibPami2025
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */



#ifndef _LIB_PAMI2025_H_
#define _LIB_PAMI2025_H_

#include "arduino.h"

#define LIB433MHZ_USED

#include "LibAff1637.h"
#include "LibJumper.h"
#include "LibMoteur.h"
#include "LibGpio.h"
#include "LibUltrason.h"
#include "LibGyroscope.h"
#include "Lib433Mhz.h"
#include "LibChrono.h"

typedef enum {
  PAMI_GPIO_1 = 0, ///< GPIO1
  PAMI_GPIO_2 = 1, ///< GPIO2
  PAMI_GPIO_3 = 2, ///< GPIO3
  PAMI_GPIO_4 = 3  ///< GPIO4
} t_pami_gpio;


/// \class LibPami2025
/// \brief Librairie de gestion du PAMI de l'année 2025
/// \details Librairies requises :
/// \details    MPU6050   : https://github.com/electroniccats/mpu6050
/// \details    Servo     : https://docs.arduino.cc/libraries/servo/
/// \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
/// \details   Wire
/// \attention En cas d'utilisation de la librairie Lib433Mhz, le timer 2 est utilisé à la fois pour gestion du 433Mhz
///            et la génération d'un signal PWM sur la broche D11 qui est utilisé pour le pilotage du moteur gauche.
/// \attention Dans le cas d'un PAMI avec un seul moteur, il faut utiliser le moteur droit (ENb)
/// \attention Dans le cas d'un moteur avec deux moteurs, il faut utiliser valider le mode setPwmMode(false)
/// \attention Dans ce dernier cas, il n'y a plus que les vitesses suivantes qui sont disponibles 0,20%,40%,60%,80%,100%
class LibPami2025
{
  public:
    LibPami2025();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments d'un PAMI
    void gestion(void);

    LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    LibGpio      gpio;      ///< Librairie de gestion des GPIO
    LibUltrason  ultrason;  ///< Librairie de gestion d'un ultrason
    LibGyroscope gyro;      ///< Librairie de gestion d'un gyroscope
    LibMoteur    moteur;    ///< Librairie de gestion d'un moteur
    Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz
    LibChrono    chrono;    ///< Librairie de gestion d'un chronometre

  private:

  protected:
};

#endif