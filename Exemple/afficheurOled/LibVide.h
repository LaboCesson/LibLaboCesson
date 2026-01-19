/*!
 * \file 	LibVide.h
 * \brief 	Le fichier de définition de la classe LibVide
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_VIDE_H_
#define _LIB_VIDE_H_


/// \class LibVideo
/// \brief Description du role de la librairie
class LibVide
{
  public:

  LibVide(
		unsigned char param1,  ///< Desciption du role du parametre 1
		unsigned char param2   ///< Desciption du role du parametre 2
	);

	/// \details Décrire le rôle de la fonction uneFonctionPublique
 	/// \return true ou false si blabla
   bool uneFonctionPublique(
		unsigned char param1,  ///< Desciption du role du parametre 1
		unsigned char param2   ///< Desciption du role du parametre 2
	);

  private:
    bool          m_debug;
   
	/// \details Décrire le rôle de la fonction uneFonctionPrivée
 	/// \return true ou false si blabla
   bool uneFonctionPrivee(
		unsigned char param1,  ///< Desciption du role du parametre 1
		unsigned char param2   ///< Desciption du role du parametre 2
	);


  protected:
};

#endif