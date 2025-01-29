/*!
 * \file 	LibPami.h
 * \brief 	Le fichier de définition de la classe LibPami
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

///< \details Librairies requises :
///< \details    MPU6050   : https://github.com/electroniccats/mpu6050
///< \details    Servo     : https://docs.arduino.cc/libraries/servo/
///< \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
///< \details   Wire


#ifndef _LIB_PAMI_H_
#define _LIB_PAMI_H_

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


/// \class LibPami
/// \brief Librairie de gestion d'un PAMI
class LibPami
{
  public:
    LibPami();

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