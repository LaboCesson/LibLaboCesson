//
// Définition du protocole de controle d'un robot bia un bus CAN
//

#ifndef _LIB_CAN_PROT_DEF_H_
#define _LIB_CAN_PROT_DEF_H_

/**
  Format général d'une commande
    1 byte : CAN_MAGIC_TAG = 0xC7
    1 bit  : 0
    3 bits : Commande
    4 bits : Longueur de la commande excluant le byte commande/longueur (n) et le Magic tag
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

  Format général d'une réponse
    1 byte : CAN_MAGIC_TAG = 0xC7
    1 bit  : 1
    3 bits : Commande reçue
    4 bits : Longueur de la réponse excluant le byte commande/longueur (n) et le Magic tag
    n bytes: Paramètres de la réponse

    commande BUS_CAN_GET_PIN_DIGITAL :
      1 byte : Numéro de pin
      1 byte : Nouvel état (LOW,HIGH)

    commande BUS_CAN_GET_PIN_ANALOGIQUE :
      1 byte : Numéro de pin
      1 byte : Poids fort de la valeur analogique
      1 byte : Poids faible de la valeur analogique

*/



#define BUS_CAN_SET_PIN_DIGITAL    1
#define BUS_CAN_GET_PIN_DIGITAL    2
#define BUS_CAN_SET_PIN_ANALOGIQUE 3
#define BUS_CAN_GET_PIN_ANALOGIQUE 4
#define BUS_CAN_SET_MOTEUR_1       5
#define BUS_CAN_SET_MOTEUR_2       6
#define BUS_CAN_DISPLAY_STRING     7
#define BUS_CAN_CUSTOM_COMMAND     10

#endif