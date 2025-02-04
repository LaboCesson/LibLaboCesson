/*!
 * \file 	LibMcp23008.h
 * \brief 	Le fichier de définition de la classe LibMcp23008
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_MCP23008_H_
#define _LIB_MCP23008_H_

#include "arduino.h"
#include "wire.h"

/// \class LibMcp23008
/// \brief Gestion du composant MCP23008 pour augmenter le nombre de pins
/// \details Les numéros de pin vont de 1 à 8
class LibMcp23008
{
  public:
    LibMcp23008(
      unsigned char addrDevice = 0x20, ///< Adresse I2C du composant
      bool doWireBegin = false         ///< si true alors la librairie doit appeler wire.begin()
    );

    /// \details Permet de valider l'utilisation du composant MCP23008
    bool begin();

    unsigned char getDeviceRegister(
      unsigned char * p_reg, ///< Pointeur vers la zone ou ecrire les valeurs lues
      unsigned char nbReg    ///< Nombre maximum de registres a lire
    );

    /// \details Permet de définir le mode d'utilisation d'une pin
    /// \details Format identique à la fonction pinMode d'arduino
    void pinMode(
      unsigned char pin, ///< Numéro de la pin a configurer (1 à 8)
      unsigned char mode ///< Mode à appliquer
    );

    /// \details Permet de définir la valeur en sortie d'une pin
    /// \details Format identique à la fonction digitalWrite d'arduino
    void digitalWrite(
      unsigned char pin,  ///< Numéro de la pin à ecrire (1 à 8)
      unsigned char value ///< Valeur à appliquer (LOW,HIGH)
    );

    /// \details Permet de lire la valeur d'une pin
    /// \details Format identique à la fonction digitalRead d'arduino
    /// \return  La valeur lue (LOW,HIGH)
    unsigned char digitalRead(
      unsigned char pin  ///< Numéro de la pin à lire (1 à 8)
    );

  private:
    void writeRegister(unsigned char reg, unsigned char data);
    unsigned char readRegister(unsigned char reg);
    bool isConnected();

    unsigned char m_addrDevice;
  protected:
};


#endif