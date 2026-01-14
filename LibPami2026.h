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
#include "LibMoteurOmni.h"
#include "LibGpio.h"
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

typedef enum {
  PAMI_COTE_BLEU  = 0, ///< Le PAMI est sur le coté Bleu du plateau
  PAMI_COTE_JAUNE = 1  ///< Le PAMI est sur le coté Jaune du plateau
} t_pami_cote_plateau;


/// \class LibPami2025
/// \brief Librairies de gestion des PAMI basiques de l'année 2026
/// \details Librairies requises :
/// \details    MPU6050   : https://github.com/electroniccats/mpu6050
/// \details    Servo     : Librairie Esp32servo par Kevin Harrington
/// \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
/// \details    Wire

class LibPami2026Basic
{
  public:
    LibPami2026Basic();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments d'un PAMI
    void gestion(void);

    /// \details Cette fonction retourne le coté du plateau ou se trouve le PAMI
    t_pami_cote_plateau get_side(void) { return m_cote_plateau; };

    LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    LibGpio      gpio;      ///< Librairie de gestion des GPIO
    LibGyroscope gyro;      ///< Librairie de gestion d'un gyroscope
    LibMoteurS   moteur;    ///< Librairie de gestion d'un couple de moteur
    Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz
    LibChrono    chrono;    ///< Librairie de gestion d'un chronometre

  private:

    t_pami_cote_plateau m_cote_plateau;
    bool m_init_led = false;

  protected:
};


/// \class LibPami2026Ninja
/// \brief Librairies de gestion d'un PAMI NINJA 2026 avec un simplecouple de moteurs 
/// \details Librairies requises :
/// \details    MPU6050   : https://github.com/electroniccats/mpu6050
/// \details    Servo     : Librairie Esp32servo par Kevin Harrington
/// \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
/// \details    Wire

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
    LibMoteur    moteur;    ///< Librairie de gestion d'un couple de moteur
    Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz
    LibChrono    chrono;    ///< Librairie de gestion d'un chronometre

  private:

  protected:
};


/// \class LibPami2026NinjaOmni
/// \brief Librairies de gestion d'un PAMI NINJA 2026 avec des roues omnidirectionnelles
/// \details Librairies requises :
/// \details    MPU6050   : https://github.com/electroniccats/mpu6050
/// \details    Servo     : Librairie Esp32servo par Kevin Harrington
/// \details    RadioHead : https://www.airspayce.com/mikem/arduino/RadioHead
/// \details    Wire

class LibPami2026NinjaOmni
{
public:
  LibPami2026NinjaOmni();

  /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments d'un PAMI
  void gestion(void);

  LibAff1637    afficheur; ///< Librairie de gestion d'un afficheur
  LibJumper     jumper;    ///< Librairie de gestion des jumpers d'un PAMI
  LibGpio       gpio;      ///< Librairie de gestion des GPIO
  LibGyroscope  gyro;      ///< Librairie de gestion d'un gyroscope
  LibMoteurOmni moteur;    ///< Librairie de gestion de roues omnidirectionnelles
  Lib433Mhz     radio;     ///< Librairie de gestion d'un récepteur 433Mhz
  LibChrono     chrono;    ///< Librairie de gestion d'un chronometre

private:

protected:
};

#endif