/*!
 * \file 	LibClavier.h
 * \brief 	Le fichier de définition de la classe LibClavier
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_CLAVIER_H_
#define _LIB_CLAVIER_H_

#include "arduino.h"
#include "LibMatrice.h"


/// \class LibClavier
/// \brief Gestion d'un clavier sous forme de matrice
/// \details La librairie peut gérer 16 touches
class LibClavier
{
  public:
    LibClavier(
      unsigned char   nbTouche,  ///< Nombre de touche à gérer
               char * p_touche,  ///< Liste des codes ASCII des touches correspondant
		  unsigned char   nbPinOut,  ///< Le nombre de lignes entrantes de la matrice du clavier
		  unsigned char * p_pinOut,  ///< La liste de pins associés aux lignes entrantes
		  unsigned char   nbPinIn,   ///< Le nombre de lignes sortantes de la matrice
		  unsigned char * p_pinIn,   ///< La liste de pins associés aux lignes sortantes
      LibMcp23008   * p_pinExt=0 ///< Pointeur vers l'objet en charge de la gestion d'un MCP23008
	  );

    /// \details Permet d'initialiser la gestion du clavier
    void begin(void);

    /// \return Retourne le code ASCII de la touche enfoncée
    char getTouche(void);
	
  private:
  unsigned char   m_nbTouche;
           char * mp_touche;
  unsigned char   m_nbDigit;
    LibMatrice  * mp_matrice;

  protected:
};

#endif