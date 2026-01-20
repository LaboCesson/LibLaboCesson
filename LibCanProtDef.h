//
// Définition du protocole de controle d'un robot bia un bus CAN
//

#ifndef _LIB_CAN_PROT_DEF_H_
#define _LIB_CAN_PROT_DEF_H_

/**
  Format général d'une commande
    1 byte : CAN_MAGIC_TAG = 0xC7
    1 bit  : 0
    7 bits : Commande
    1 byte : Longueur de la commande excluant le byte commande/longueur (n) et le Magic tag
    n bytes: Paramètres de la commande

    commande BUS_CAN_SET_PIN_DIGITAL :
      1 byte : Numéro de pin
      1 byte : Nouvel état (LOW,HIGH)

    commande BUS_CAN_GET_PIN_DIGITAL :
      1 byte : Numéro de pin

    commande BUS_CAN_SET_PIN_ANALOGIQUE :
      1 byte : Numéro de pin
      1 byte : Poids fort de la valeur analogique
      1 byte : Poids faible de la valeur analogique

    commande BUS_CAN_GET_PIN_ANALOGIQUE :
      1 byte : Numéro de pin

    commande BUS_CAN_SET_MOTEUR_1 :
    commande BUS_CAN_SET_MOTEUR_2 :
      1 byte : vitesse du moteur Gauche (-100 <-> 100, 0=stop)
      1 byte : vitesse du moteur Droit (-100 <-> 100, 0=stop)

    commande BUS_CAN_DISPLAY_STRING :
      1 bytes : Nombre de caratère à afficher (n)
      n bytes : Code ASCII des caractères à afficher

    commande BUS_CAN_GET_COLOR :
      1 bytes : Nombre de couleurs à afficher

    commande BUS_CAN_SET_PWM :
      1 byte : Numéro de GPIO (0-5)
      1 byte : Angle (0-90/180)

  Format général d'une réponse
    1 byte : CAN_MAGIC_TAG = 0xC7
    1 bit  : 1
    7 bits : Commande reçue
    1 byte : Longueur de la réponse excluant le byte commande/longueur (n) et le Magic tag
    n bytes: Paramètres de la réponse

    commande BUS_CAN_GET_PIN_DIGITAL :
      1 byte : Numéro de pin
      1 byte : Nouvel état (LOW,HIGH)

    commande BUS_CAN_GET_PIN_ANALOGIQUE :
      1 byte : Numéro de pin
      1 byte : Poids fort de la valeur analogique
      1 byte : Poids faible de la valeur analogique

    commande BUS_CAN_GET_COLOR :
      2 bits : Couleur détecteur 1
      2 bits : Couleur détecteur 2
      2 bits : Couleur détecteur 3
      2 bits : Couleur détecteur 4
        typedef t_robot_color
        ROBOT_COULEUR_INCONNUE = 0 : Aucune couleur
        ROBOT_COULEUR_JAUNE    = 1 : Couleur Jaune
        ROBOT_COULEUR_BLEU     = 2 : Couleur Bleu
*/

#define BUS_CAN_SET_PIN_DIGITAL    1
#define BUS_CAN_GET_PIN_DIGITAL    2
#define BUS_CAN_SET_PIN_ANALOGIQUE 3
#define BUS_CAN_GET_PIN_ANALOGIQUE 4
#define BUS_CAN_SET_MOTEUR_1       5
#define BUS_CAN_SET_MOTEUR_2       6
#define BUS_CAN_DISPLAY_STRING     7
#define BUS_CAN_GET_COLOR          8
#define BUS_CAN_SET_GPIO_PWM       9
#define BUS_CAN_CUSTOM_COMMAND     10

#endif