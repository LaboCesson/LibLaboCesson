/*!
 * \file 	LibMatrice.h
 * \brief 	Le fichier de définition de la classe LibMatrice
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_MATRICE_H_
#define _LIB_MATRICE_H_

#include "arduino.h"
#include "LibMcp23008.h"


/// \class LibMatrice
/// \brief Gestion d'une matrice de contacts
/// \details La librairie peut gérer 16 contacts
class LibMatrice
{
  public:
    LibMatrice(
		unsigned char   nbPinOut,  ///< Le nombre de lignes entrantes de la matrice
		unsigned char * p_pinOut,  ///< La liste de pins associés aux lignes entrantes
		unsigned char   nbPinIn,   ///< Le nombre de lignes sortantes de la matrice
		unsigned char * p_pinIn,   ///< La liste de pins associés aux lignes sortantes
    LibMcp23008   * p_pinExt=0 ///< Pointeur vers l'objet en charge de la gestion d'un MCP23008
	  );

    /// \details Permet d'initialiser la gestion de la matrice
    void begin(void);

    /// \details Sous la forme d'un short 1 bit par contact
    /// \details Le bit de poids faible correspond au contact correspondant à la première ligne et première colonne
    /// \details Le bit suivant correspond au contact suivant de la ligne et ainsi de suite
    unsigned short getContacts(void);

    /// \details Retourne l'index du premier contact détecté
    unsigned char getIndex(void);

    /// \details Retourne le nombre de contacts gérés par la matrice
    unsigned char getNbContacts(void);

  private:
    unsigned char   m_nbPinOut;
    unsigned char   m_nbPinIn;
    unsigned char * mp_pinOut;
    unsigned char * mp_pinIn;
    unsigned short  m_contact;
    LibMcp23008 *   mp_pinExt;

    void setPinMode(unsigned char pin, unsigned char mode);
    void setPinOut(unsigned char pin, unsigned char val);
    unsigned char readPin(unsigned char pin);

  protected:
};

#endif