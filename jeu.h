#ifndef _JEU_H
#define _JEU_H

/* Prototypes */
void play(SDL_Surface *ecran);
void deplacer(int carte[][HAUTEUR], SDL_Rect *pos, int direction);
void deplacer_box(int *p_case, int *d_case);

#endif
