/* Crée le : 01/06/2013 */
/* Modifié le : 03/06/2013 */
/* Commentaires : La gestion des collisions est la bete noir des programmeurs ! */
/* Idée d'amélioration : Ajouter des MSG_Box de <windows.h> pour gérer les erreurs */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "const.h"
#include "jeu.h"
#include "edit.h"
#include "level.h"


/* Fonction principale */
int main(int argc, char *argv[])
{
SDL_Surface *ecran=NULL, *menu=NULL;
SDL_Rect pos_menu;
SDL_Event event;
bool continuer=1;
/* Initialisations */
if (SDL_Init(SDL_INIT_VIDEO)==-1) /* S'il y'a une erreur */
{
fprintf(stderr, "Erreur d'initialisation du mode video : %s\n", SDL_GetError());                                 
exit(EXIT_FAILURE);
}
SDL_WM_SetIcon(IMG_Load("ressources\\box_o.png"), NULL);
ecran=SDL_SetVideoMode(F_LARGEUR, F_HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
SDL_WM_SetCaption("Open The Box", NULL);
menu=IMG_Load("ressources\\menu.png");
pos_menu.x=0;
pos_menu.y=0;
/* Gestion d'évenements */
while(continuer)
{
SDL_WaitEvent(&event);
  switch (event.type)
  {
  case SDL_QUIT: continuer=0; break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE: continuer=0; break;
    case SDLK_KP1: play(ecran); break;
    case SDLK_KP2: edit(ecran); break; 
    case SDLK_KP3: reset(ecran, 1); break;
    } break;
  }
/* Blit */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 128, 102, 74));
SDL_BlitSurface(menu, NULL, ecran, &pos_menu);
/* Mise a jour de l'ecran */
SDL_Flip(ecran);
}
/* Vidage de la mémoire */
SDL_FreeSurface(menu);
SDL_Quit();
return EXIT_SUCCESS;
}
