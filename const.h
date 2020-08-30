#ifndef _CONST_H
#define _CONST_H

/* Les constantes */
#define TAILLE 32         /* Taille de l'image */
#define HAUTEUR 12       
#define LARGEUR 12
#define F_HAUTEUR TAILLE * HAUTEUR
#define F_LARGEUR TAILLE * LARGEUR
enum{HAUT, BAS, GAUCHE, DROITE};   /* Les directions */
enum{VIDE, BRIQUE, BOX, CLEF, PERSO, BOX_OK};
typedef enum booleen {FALSE, TRUE} bool; /* On défini un type de variable booléen */

#endif
