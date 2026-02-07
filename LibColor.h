/*!
 * \file 	LibColor.h
 * \brief 	Le fichier de définition des classes gérant les détecteurs de couleur
 * \author  LaboCesson
 * \version 1.0
 * \date    Novembre 2025
 *
 * \details Ce fichier contient le source des classes suivantes
 * \details - \ref LibTcs3472
 * \details - \ref LibMultiTcs3472
 */

#ifndef _LIB_COLOR_H_
#define _LIB_COLOR_H_

#include "Adafruit_TCS34725.h"
#include "LibMuxI2c.h"

typedef enum {
  ROBOT_COULEUR_INCONNUE = 0, ///< Couleur Inconnue détectée
  ROBOT_COULEUR_JAUNE    = 1, ///< Couleur Jaune détectée
  ROBOT_COULEUR_BLEU     = 2, ///< Couleur Bleu détectée
} t_robot_color;


/// \class LibTcs3472
/// \brief Description de la librairie de gestion d'un détecteur de couleur TCS3472
/// \details Le composant TCS3472 permet de détecter les couleurs
/// \details Il permet de retourner les valeurs RGB et la temperature de la couleur
/// \details Librairies requises : Adafruit_TCS34725

class LibTcs3472
{
  public:
		LibTcs3472(void);

		/// \details Permet d'initialiser et de valider la gestion du tcs3472
		/// \return  Retourne true si l'initialisation s'est bien passée
		bool begin(void);

    /// \details Cette fonction doit être appelée régulièrement pour la gestion du détecteur de couleur
    void gestion(void);

		/// \details Permet de lire la temperature de la couleur lue
		/// \return Retourne la valeur de la temperature
		/// - Temperature froide : typiquement 9000 pour du bleu
		/// - Temperature chaude : typiquement 3000 pour du Jaune
		/// Retourne 0 si la temperature est inconnue
		unsigned short getTemperature(void);

		/// \details Permet de lire les valeurs R,G,et B de la couleur lue
		void getRGB(
			unsigned char * p_red,   ///< Pointeur ou écrire la valeur du rouge
			unsigned char * p_green, ///< Pointeur ou écrire la valeur du vert
			unsigned char * p_blue 	 ///< Pointeur ou écrire la valeur du bleu
		);

		/// \details Retourne la couleur détectée
		/// - \ref ROBOT_COULEUR_INCONNUE : la couleur n'est ni bleu ni jaune
		/// - \ref ROBOT_COULEUR_JAUNE : la couleur est jaune
		/// - \ref ROBOT_COULEUR_BLEU : la couleur est bleu
		t_robot_color getColor(void);

		/// \details Permet de valider l'affichage de message de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		);

  private:
    bool m_debug;
    bool m_begin;
		int  m_period;

  	unsigned long  m_nextTimeGestion;

		unsigned short m_red,m_green,m_blue,m_chrominance;
		unsigned short m_temperature;

		Adafruit_TCS34725 tcs; ///< Librairie de gestion d'un TCS3472
  
		void trace(char * msg);
		// void trace(char * msg, int val);

		//int m_status;

  protected:
};


/// \class LibMultiTcs3472
/// \brief Description de la librairie de gestion de plusieurs détecteurs de couleur TCS3472
/// \details L'adresse I2C du composant TCS3472 est égal à 0x29 et ne peut être modifié
/// \details On utilise donc un multiplexeur I2C pour gérer plusieurs composants
/// \details Librairies requises : Adafruit_TCS34725, \ref LibTcs3472

#define NB_MAX_MULTI_TCS3472 8

class LibMultiTcs3472
{
	public:
		LibMultiTcs3472(void);

		/// \details Permet d'initialiser et de valider la gestion des tcs3472
		/// \return  Retourne true si l'initialisation s'est bien passée
		bool begin(
			int             nbColor, ///< Nombre de détecteur à gérer (Maximum NB_MAX_MULTI_TCS3472)
			unsigned char * muxIndex ///< Liste des entrées du multipleur associée à chaque TCS3472
		);

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des détecteurs de couleur
    void gestion(void);

		/// \details Retourne les couleurs détectées
		/// - \ref ROBOT_COULEUR_INCONNUE : la couleur n'est ni bleu ni jaune
		/// - \ref ROBOT_COULEUR_JAUNE : la couleur est jaune
		/// - \ref ROBOT_COULEUR_BLEU : la couleur est bleu
		void getColors(
			int            nbColor,   ///< Nombre de couleur à retourner (Maximum NB_MAX_MULTI_TCS3472)
			t_robot_color* p_tabColor ///< pointeur vers le tableau de couleur à retourner
		);

		/// \details Permet de valider l'affichage de message de debug
		void setDebug(
			bool debug ///< si true les messages de debug sont affichés
		);

  private:
    bool m_debug;
    bool m_begin;
		int  m_nbColor;

		LibMuxI2c m_mux;

		LibTcs3472 *  m_tabColor;
		unsigned char m_muxIndex[NB_MAX_MULTI_TCS3472];

		void trace(char * msg);

  protected:
};

#endif