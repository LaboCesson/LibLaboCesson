/*!
 * \file 	LibMuxI2c.h
 * \brief 	Le fichier de définition de la classe LibMuxI2c
 * \author  LaboCesson
 * \version 1.0
 * \date    Décembre 2025
 */

#ifndef _LIB_MUX_I2C_H_
#define _LIB_MUX_I2C_H_

#include "arduino.h"
#include "Wire.h"


#define DEFAULT_TCA9548A_ADDRESS 0x70
#define DEFAULT_SELECTED_LINE    0


/// \class LibMuxI2c
/// \brief Gestion du multiplexeur I2C TCA9548A 
/// \details Le multiplexeur I2C TCA9548A permet de gérer jusqu'à 8 lignes I2C
/// \details Cela permet de gérer simultanément plusieurs composants I2C possédant la même adresse
/// \details La classe LibMuxI2c permet de sélectionner une des 8 voies I2C avant de pouvoir dialoguer
///          avec un des composants présents sur la ligne sélectionnée
/// \details A l'initialisation, la ligne 0 est sélectionnée
class LibMuxI2c
{
  public:

  LibMuxI2c(
		unsigned char muxAddress = DEFAULT_TCA9548A_ADDRESS   ///< Permet de sélectionner une adresse différente de l'adresse par défaut
	);

	/// \details Permet de sélectionner une ligne I2C
  void selectLine(
		unsigned char i2cLine ///< Numéro de la ligne I2C à sélectionner (0-7);
	);

	/// \details Permet de retourner la ligne I2C sélectionnée
  unsigned char getSselectedLine(void) { return m_line; }

	/// \details Permet de valider l'affichage de message de debug
	void setDebug(
		bool debug ///< si true les messages de debug sont affichés
	) { m_debug = debug; }

  private:
    bool          m_debug;
		unsigned char m_muxAddress;
		unsigned char m_line;
  protected:
};

#endif