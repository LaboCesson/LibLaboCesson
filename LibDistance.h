/*!
 * \file 	LibDistance.h
 * \brief 	Le fichier de définition de la classe LibDistance
 * \author  LaboCesson
 * \version 1.0
 * \date    Avril 2026
 */

#ifndef _LIB_DISTANCE_H_
#define _LIB_DISTANCE_H_

#include "arduino.h"

#define LIBDISTANCE_MAX_SECTION 20 // Nombre maximum de sections

typedef struct {
	unsigned int  nbPoint;		     ///< Nombre de point de détection
	unsigned int  pinDetector;     ///< Numéro de pin associé au détecteur
	unsigned int  circonference;   ///< Circonference de la roue en dixième de mm
	unsigned int  sectionSize;     ///< Distance entre deux détection en dixième de mm
	bool          levelDetector;   ///< Etat LOW/HIGH géré pour la détection
	unsigned int  dureeAntirebond; ///< Durée maximum de l'antirebond en ms

 	unsigned long detectionTime[LIBDISTANCE_MAX_SECTION]; ///< Table mémorisant les derniers temps de détection
	unsigned int  indexTable; ///< Pointeur dans la table vers le prochain emplacement ou ranger en temps de détection
	unsigned int  nbTime;     ///< Nombre de temps dans la table (entre 0 et nbPoint)

  unsigned long lastInterrupt;      ///< Heure de la dernière interruption (front montant ou descendant)
  unsigned long launchingTime;      ///< Heure du lancement de calcul de la distance
	unsigned long lastDetectionTime;  ///< Heure de la derniere détection
  unsigned int  nbDetection;        ///< Cumul du nombre de détection
	unsigned int  lastVitesse;        ///< Vitesse en mm/sec de la dernière section
	unsigned int  firstVitesse;       ///< Vitesse en mm/sec de la première section
	unsigned int  vitesseMoyenne;     ///< Vitesse moyenne en mm/sec du dernier tour
	unsigned long firstSectionTime;   ///< Durée avant la première détection
	//unsigned int  firstSectionSize;   ///< Distance lors de la première section incomplète en dixième de mm
	unsigned int  debug;
} t_LibDistance_work;


/// \class LibDistance
/// \brief Cette librairie permet de gérer la distance parcouru à l'aide d'un moteur ou deux moteurs
/// \details \todo
class LibDistance
{
  public:

		LibDistance(
			unsigned int pinRoueGauche, ///< Numéro du pin associé au détecteur de la roue gauche
			unsigned int pinRoueDroite ///< Numéro du pin associé au détecteur de la roue droite
		);

		/// \details Permet de valider le calcul des distances
		void begin(
			unsigned int diametre,       ///< Diametre de la roue associée au moteur en mm
			unsigned int nbPoint,        ///< Nombre de points de détection présents sur la roue
			bool         levelDetector,  ///< Etat LOW/HIGH géré pour la détection
			unsigned int dureeAntirebond ///< Durée maximum de l'antirebond en ms (typiquement 5ms)
		);

		/// \details Permet d'indiquer la vitesse par défaut pour la première section
		void setDefaultSpeed(
			unsigned defaultSpeed ///< vitesse par défaut pour la première section en mm/sec
		) { m_defaultSpeed = defaultSpeed; }

		/// \details Permet de valider le début de mesure d'une distance du moteur gauche
		void startDistance();

		/// \details Permet de valider le début de mesure d'une distance du moteur gauche
		void startDistanceGauche();

		/// \details Permet de valider le début de mesure d'une distance du moteur droit
		void startDistanceDroite();

		/// \details Permet de connaitre la distance depuis son initialisation
		/// \return  Retourne la distance parcouru en mm
		unsigned int getDistance();

		/// \details Permet de valider l'affichage de messages de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		) { m_debug = debug; }

  private:
    bool m_debug = false;
		bool m_begin = false;

		unsigned int m_diametre;
		unsigned int m_nbPoint;
		unsigned int m_pinRoueGauche;
		unsigned int m_pinRoueDroite;
		bool         m_levelDetector;
		unsigned int m_dureeAntirebond;
		unsigned int m_defaultSpeed;  // Vitesse par défaut en mm/sec

		static void IRAM_ATTR gestionIntRoueDroite(); ///< \details Fonction appelée lors de la détection d'un front coté droit
		static void IRAM_ATTR gestionIntRoueGauche(); ///< \details Fonction appelée lors de la détection d'un front coté gauche

		/// \details Gestion de la détection d'un front pour une roue
		static void IRAM_ATTR gestionIntRoue( 
			t_LibDistance_work * p_ctx ///< Pointeur vers le contexte de gestion de la roue
		);

		/// \details Permet de connaitre la distance depuis son initialisation
		/// \return  Retourne la distance parcouru en mm
		unsigned int getWheelDistance(
			t_LibDistance_work* p_ctx ///< Pointeur vers le contexte de gestion de la roue
		);

		/// \details Initialise le contexte d'une roue
		void initCtxRoue(
			t_LibDistance_work * p_ctx, ///< Pointeur vers le contexte de gestion de la roue
			unsigned int pinDetector ///< Numéro du pin associé au détecteur
		);

		void printInfo(
			t_LibDistance_work* p_ctx,   ///< Pointeur vers le contexte de gestion de la roue
			unsigned int        distance ///< Distance estimée
		);

  protected:
};

#endif