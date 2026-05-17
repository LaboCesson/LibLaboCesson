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
} t_LibDistance_ctx;


/// \class LibDistance
/// \brief Cette librairie permet de mesurer la distance parcourue
/// \details
/// Il est possible d'utiliser un ou deux détecteurs associés chacun à une roue.
/// Lorsque deux roues sont utilisées, la distance est calculée en divisant par deux la somme des deux distances calculées.
/// La distance correspond alors à la distance parcourue par le point situé au milieu des deux roues.<BR>
/// Le calcul des distances n'est possible qu'une fois la fonction \ref begin appelée.
/// 
/// Cet appel valide entre-autres l'activation de la gestion des interruptions.
/// 
/// Un calcul de distance est initié via la fonction \ref startDistance.<BR>
/// La distance calculée est obtenu via la fonction \ref getDistance.
/// 
/// La calcul de la distance se fait en multipliant la distance entre deux points de détection 
/// par le nombre de détection comptabilisé moins 1
/// et en y ajoutant la distance D1 avant le premier point de détection et D2 celle après le dernier point de détection.<BR>
/// Ces deux distances D1 et D2 sont une estimation:<BR>
/// - La distance D1 est calculée grace au temps mesuré entre l'heure d'appel de la fonction \ref startDistance
///   et l'heure de la première détection et une vitesse V1.<BR>
///   Cette vitesse V1 est la vitesse calculée entre les deux premiers point de détection
///   ou une vitesse par défaut réglable via la fonction \ref setDefaultSpeed si moins de 2 points de détection ont été rencontrés.<BR>
/// - La distance D2 est calculé grace au temps mesuré entre l'heure de la dernière détection 
///   et l'heure d'appel de la fonction \ref getDistance et une vitesse V2.<BR>
///   Cette vitesse V1 est la vitesse moyenne calculée sur l'ensemble du parcours
///   ou une vitesse par défaut réglable via la fonction \ref setDefaultSpeed si moins de 2 points de détection ont été rencontrés.<BR>
/// 
/// Afin d'avoir une estimation la plus proche de la réalité, il convient
/// - d'appeler la fonction \ref startDistance à un moment le plus proche possible du démarrage des moteurs<BR>
///	- d'arreter dès que possible les moteurs lorsqu'une distance attendue est retournée<BR>
/// - d'éviter un trajet trop court avec moins de deux points de détection<BR>
/// 
/// \attention
/// Lorsque la librairie \ref LibMoteurS est utilisée pour la gestion des moteurs,
/// le démarrage effectif des moteurs peut-être différé.<BR>
/// L'appel de la fonction \ref startDistance est délégué à la cette librairie.<BR>
/// Pour que cette délégation soit effective une fonction est prévue dans la librairie \ref LibMoteurS
/// 
class LibDistance
{
  public:

		LibDistance(
			unsigned int pinRoueGauche, ///< Numéro du pin associé au détecteur de la roue gauche (0 si non utilisé)
			unsigned int pinRoueDroite  ///< Numéro du pin associé au détecteur de la roue droite (0 si non utilisé)
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

		/// \details Permet de valider le début de mesure d'une distance
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
			t_LibDistance_ctx* p_ctx ///< Pointeur vers le contexte de gestion de la roue
		);

		/// \details Permet de connaitre la distance depuis son initialisation
		/// \return  Retourne la distance parcouru en mm
		unsigned int getWheelDistance(
			t_LibDistance_ctx* p_ctx ///< Pointeur vers le contexte de gestion de la roue
		);

		/// \details Initialise le contexte d'une roue
		void initCtxRoue(
			t_LibDistance_ctx* p_ctx, ///< Pointeur vers le contexte de gestion de la roue
			unsigned int pinDetector ///< Numéro du pin associé au détecteur
		);

		void printInfo(
			t_LibDistance_ctx* p_ctx,   ///< Pointeur vers le contexte de gestion de la roue
			unsigned int        distance ///< Distance estimée
		);

  protected:
};

#endif