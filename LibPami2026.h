/*!
 * \file 	LibPami2026.h
 * \brief 	Le fichier de définition de la classe LibPami2026
 * \author  LaboCesson
 * \version 1.0
 * \date    Octobre 2026
 */



#ifndef _LIB_PAMI2026_H_
#define _LIB_PAMI2026_H_

#include "arduino.h"

#include "LibAff1637.h"
#include "LibJumper.h"
#include "LibMoteur.h"
#include "LibGpio.h"
// #include "LibUltrason.h"
#include "LibGyroscope.h"
#include "Lib433Mhz.h"
#include "LibChrono.h"

typedef enum {
  PAMI_GPIO_1 = 0, ///< GPIO1
  PAMI_GPIO_2 = 1, ///< GPIO2
  PAMI_GPIO_3 = 2, ///< GPIO3
  PAMI_GPIO_4 = 3, ///< GPIO4
  PAMI_GPIO_5 = 4, ///< GPIO5
  PAMI_GPIO_6 = 5  ///< GPIO6
} t_pami_gpio;


/// \class LibPami2025
/// \brief Librairies de gestion des PAMI de l'année 2026
/// \details Librairies requises :
/// \details    MPU6050   : https://github.com/electroniccats/mpu6050
/// \details    Servo     : https://docs.arduino.cc/libraries/servo/
/// \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
/// \details   Wire

class LibPami2026Basic
{
  public:
    LibPami2026Basic();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments d'un PAMI
    void gestion(void);

    LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    LibGpio      gpio;      ///< Librairie de gestion des GPIO
    LibGyroscope gyro;      ///< Librairie de gestion d'un gyroscope
    LibMoteurS   moteur;    ///< Librairie de gestion d'un couple de moteur
    Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz
    LibChrono    chrono;    ///< Librairie de gestion d'un chronometre

  private:

  protected:
};

class LibPami2026Ninja
{
  public:
    LibPami2026Ninja();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments d'un PAMI
    void gestion(void);

    LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    LibGpio      gpio;      ///< Librairie de gestion des GPIO
    LibGyroscope gyro;      ///< Librairie de gestion d'un gyroscope
    LibMoteur    moteur1;   ///< Librairie de gestion d'un couple de moteur
    LibMoteur    moteur2;   ///< Librairie de gestion d'un couple de moteur (cas omnidirectionnel)
    Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz
    LibChrono    chrono;    ///< Librairie de gestion d'un chronometre

  private:

  protected:
};


#endif