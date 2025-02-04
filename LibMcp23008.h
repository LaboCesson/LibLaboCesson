/*!
 * \file 	LibMcp23008.h
 * \brief 	Le fichier de d�finition de la classe LibMcp23008
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
/// \details Les num�ros de pin vont de 1 � 8
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

    /// \details Permet de d�finir le mode d'utilisation d'une pin
    /// \details Format identique � la fonction pinMode d'arduino
    void pinMode(
      unsigned char pin, ///< Num�ro de la pin a configurer (1 � 8)
      unsigned char mode ///< Mode � appliquer
    );

    /// \details Permet de d�finir la valeur en sortie d'une pin
    /// \details Format identique � la fonction digitalWrite d'arduino
    void digitalWrite(
      unsigned char pin,  ///< Num�ro de la pin � ecrire (1 � 8)
      unsigned char value ///< Valeur � appliquer (LOW,HIGH)
    );

    /// \details Permet de lire la valeur d'une pin
    /// \details Format identique � la fonction digitalRead d'arduino
    /// \return  La valeur lue (LOW,HIGH)
    unsigned char digitalRead(
      unsigned char pin  ///< Num�ro de la pin � lire (1 � 8)
    );

  private:
    void writeRegister(unsigned char reg, unsigned char data);
    unsigned char readRegister(unsigned char reg);
    bool isConnected();

    unsigned char m_addrDevice;
  protected:
};


#endif