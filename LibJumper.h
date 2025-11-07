/*!
 * \file 	LibJumper.h
 * \brief 	Le fichier de définition de la classe LibJumper
 * \author  LaboCesson
 * \version 2.0
 * \date    Octobre 2025
 */

#ifndef _LIB_JUMPER_H_
#define _LIB_JUMPER_H_

#include "arduino.h"

#define MAX_NB_JUMPERS 8

typedef enum {
      PAMI_TEAM_A = 0, ///< Jumper non positionné  :..
      PAMI_TEAM_B = 1  ///< Jumper positionne      |..
} t_pamiTeam;



/// \class LibJumper
/// \brief Gestion des Jumpers d'un PAMI
class LibJumper
{
  public:
    LibJumper(
		unsigned char  pinJumperTeam, ///< Le numéro de la pin connectée au Jumper Team
    unsigned char* jumpersPinList, ///< Pointeur vers la liste de pin de type Jumper
    unsigned char  nbPinInTab     ///< Nombre de pins dans la liste
    );

	/// \details Retourne le numéro de l'équipe auquel appartient le PAMI
	/// - \ref PAMI_TEAM_A pour l'équipe A
	/// - \ref PAMI_TEAM_B pour l'équipe B
    t_pamiTeam getTeam(void);
	
	/// \details Retourne le numéro du PAMI
     unsigned char getPami(void);

  private:
    unsigned char m_pinJumperTeam;
    unsigned char m_nbJumpers;
    unsigned char m_jumpers[MAX_NB_JUMPERS];

  protected:
};


#endif