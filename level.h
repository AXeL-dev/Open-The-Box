#ifndef _LEVEL_H
#define _LEVEL_H

/* Prototypes */
int charger_niveau(int niveau[][HAUTEUR], int level);
int sauvegarder_niveau(int niveau[][HAUTEUR]);
void reset(SDL_Surface *ecran, bool afficher);

#endif
