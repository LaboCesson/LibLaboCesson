/*!
 * \file 	LibMoteurOmni.h
 * \brief 	Le fichier de définition de la classe LibVide
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_MOTEUR_OMNI_H_
#define _LIB_MOTEUR_OMNI_H_

typedef enum {
  STOP      = 0, ///<
  DEGRE_0   = 1, ///<
  DEGRE_45  = 2, ///<
  DEGRE_90  = 3, ///<
  DEGRE_135 = 4, ///<
  DEGRE_180 = 5, ///<
  DEGRE_225 = 6, ///<
  DEGRE_270 = 7, ///<
  DEGRE_315 = 8, ///<
  TOURNE_GAUCHE =  9, ///<
  TOURNE_DROITE = 10  ///<
} t_omni_direction;


#include "LibMoteur.h"

typedef enum {
    OMNI_TOURNE_GAUCHE = 0, ///< Le PAMI doit tourner à gauche
    OMNI_TOURNE_DROITE = 1  ///< Le PAMI doit tourner à droite
} t_omni_sens;

/// \class LibMoteurOmni
/// \brief Description du role de la librairie LibMoteurOmni
class LibMoteurOmni
{
  public:

    LibMoteur    moteur1;   ///< Librairie de gestion d'un couple de moteur
    LibMoteur    moteur2;   ///< Librairie de gestion d'un couple de moteur (cas omnidirectionnel)

		LibMoteurOmni(void);

		/// \details Permet de controler le moteur avant gauche
		void avantGauche(
			int vitesse   ///< Vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
		);

		/// \details Permet de controler le moteur avant droit
		void avantDroit(
			int vitesse   ///< Vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
		);

		/// \details Permet de controler le moteur arriere gauche
		void arriereGauche(
			int vitesse   ///< Vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
		);

		/// \details Permet de controler le moteur arriere droit
		void arriereDroit(
			int vitesse   ///< Vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
		);

		/// \details Permet de faire avancer le robot dans une direction
		void avance(
			t_omni_direction direction, ///< Direction en degré vers ou faire avancer le robot
			int              vitesse    ///< Vitesse du robot
		);

		/// \details Permet d'arreter le robot
		void stop( void );

		/// \details Permet de valider l'affichage de message de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		);

  private:
    bool m_debug;

  protected:
};

#endif