/*!
 * \file 	  LibPRobot2026.h
 * \brief 	Le fichier de définition des classes de gestion des robots 2026
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2026
 */



#ifndef _LIB_ROBOT2026_H_
#define _LIB_ROBOT2026_H_

#include "LibCan.h"
#include "LibMoteur.h"
#include "LibChrono.h"
#include "LibColor.h"


class LibRobot2026Robot1Recv
{
  public:
    LibRobot2026Robot1Recv();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments du robot
    void gestion(void);

    LibCan2515     canBus;
    LibCanProtRecv canProt;
    LibMoteur      moteur1;
    LibMoteur      moteur2;
    LibGpio        gpio;
    //LibTcs3472 color;
    //LibChrono  chrono;

    // LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    // LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    // LibGyroscope gyro;      ///< Librairie de gestion d'un gyroscope
    // LibMoteurS   moteur;    ///< Librairie de gestion d'un couple de moteur
    // Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz

  private:

  protected:
};


class LibRobot2026Robot1Send
{
  public:
    LibRobot2026Robot1Send();

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des différents éléments du robot
    void gestion(void);

    LibCan2515     canBus;
    LibCanProtSend canProt;
    LibChrono      chrono;

    // LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    // LibJumper    jumper;    ///< Librairie de gestion des jumpers d'un PAMI
    // LibGpio      gpio;      ///< Librairie de gestion des GPIO
    // Lib433Mhz    radio;     ///< Librairie de gestion d'un récepteur 433Mhz

  private:

  protected:
};

#endif