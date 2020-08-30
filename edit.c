#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "const.h"
#include "edit.h"
#include "level.h"

void edit(SDL_Surface *ecran)
{
SDL_Surface *brique=NULL, *box=NULL, *clef=NULL, *perso=NULL, *box_ok=NULL, *objet=NULL;
SDL_Surface *edit_instruction=NULL, *saved=NULL;
SDL_Rect pos, pos_objet, pos_e_instruction, pos_saved;
SDL_Event event;
bool continuer=1, clic_g=0, clic_d=0, perso_creer=0, c_e_instruction=1;
int objet_actuel=BRIQUE, i=0, j=0, carte[LARGEUR][HAUTEUR]={0};
/* Les sprites pour l'édition */
brique=IMG_Load("ressources\\brique.png");
box=IMG_Load("ressources\\box.png");
clef=IMG_Load("ressources\\clef.png");
perso=IMG_Load("ressources\\perso_b.png");
box_ok=IMG_Load("ressources\\box_o.png");
/* Activation de la transparance du perso */
SDL_SetColorKey(perso, SDL_SRCCOLORKEY, SDL_MapRGB(perso->format, 255, 255, 255));
/* Les sprites d'instructions */
edit_instruction=IMG_Load("ressources\\edit_instruction.png");
saved=IMG_Load("ressources\\saved.png");
pos_e_instruction.x=0;
pos_e_instruction.y=0;
SDL_BlitSurface(edit_instruction, NULL, ecran, &pos_e_instruction);
/* Mise a jour de l'ecran apres blit des instructions */
SDL_Flip(ecran);
/* Attente d'appui sur une touche pour commencer le jeu */
while (c_e_instruction)
{
SDL_WaitEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT: c_e_instruction=0; continuer=0; break;
    case SDL_KEYDOWN: 
         switch (event.key.keysym.sym)
         {
         case SDLK_ESCAPE: c_e_instruction=0; continuer=0; break;
         default: c_e_instruction=0; SDL_FreeSurface(edit_instruction); break;
         } break;
    }
}
/* Placement de la sourie au milieu de l'ecran */
SDL_WarpMouse(ecran->w/2, ecran->h/2);
/* Position de l'objet actuel au milieu de l'ecran comme la sourie */
pos_objet.x=ecran->w/2;
pos_objet.y=ecran->h/2; 
/* Chargement de la carte */  
if (!charger_niveau(carte, 1)) 
{
fprintf(stderr, "Erreur lors de l'edition du niveau.\n");
exit(EXIT_FAILURE);
}
while (continuer)
{
SDL_WaitEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT: continuer=0; break;
    case SDL_MOUSEBUTTONDOWN:
         if (event.button.button==SDL_BUTTON_LEFT)
         {
         if (objet_actuel==PERSO && perso_creer==1) break; /* S'il y'a déjà un perso poser sur la carte */
         if (carte[event.button.x / TAILLE][event.button.y / TAILLE]==PERSO) perso_creer=0; /* Si on change la case du perso par autre chose */
         carte[event.button.x / TAILLE][event.button.y / TAILLE]=objet_actuel;
         clic_g=1;
         }
         else if(event.button.button==SDL_BUTTON_RIGHT)
         {
         if (carte[event.button.x / TAILLE][event.button.y / TAILLE]==PERSO) perso_creer=0; /* Si on efface le perso */
         carte[event.button.x / TAILLE][event.button.y / TAILLE]=VIDE;
         clic_d=1;
         } break;
     case SDL_MOUSEBUTTONUP:
          if (event.button.button==SDL_BUTTON_LEFT) clic_g=0;
          else if(event.button.button==SDL_BUTTON_RIGHT) clic_d=0;
     break;
     case SDL_MOUSEMOTION:
          pos_objet.x=event.motion.x; pos_objet.y=event.motion.y;
          if (clic_g) 
          { 
             if (objet_actuel==PERSO && perso_creer==1) break;
             carte[event.motion.x / TAILLE][event.motion.y / TAILLE]=objet_actuel;
          }
          else if (clic_d) 
          {  
             if (carte[event.button.x / TAILLE][event.button.y / TAILLE]==PERSO) perso_creer=0;
             carte[event.motion.x / TAILLE][event.motion.y / TAILLE]=VIDE; 
          }
     break;
     case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
          {
          case SDLK_ESCAPE: continuer=0; break;
          case SDLK_s: sauvegarder_niveau(carte); /* Sauvegarde */
          /* Placement au milieu + Blit */
          pos_saved.x=(ecran->w/2)-(saved->w/2);
          pos_saved.y=(ecran->h/2)-(saved->h/2);
          SDL_BlitSurface(saved, NULL, ecran, &pos_saved); 
          /* Mise a jour de l'ecran */
          SDL_Flip(ecran);
          /* Initialisation */
          c_e_instruction=1;
              while (c_e_instruction)
              {
              SDL_WaitEvent(&event);
                  switch(event.type)
                  {
                  case SDL_QUIT: c_e_instruction=0; continuer=0; break;
                  case SDL_KEYDOWN: 
                       switch (event.key.keysym.sym)
                       { 
                       case SDLK_ESCAPE: c_e_instruction=0; continuer=0; break;
                       default: c_e_instruction=0; break;
                       } break;
                  }
               }
          break;
          case SDLK_r: charger_niveau(carte, 1); break;
          case SDLK_KP1: objet_actuel=BRIQUE; break;
          case SDLK_KP2: objet_actuel=BOX; break;
          case SDLK_KP3: objet_actuel=CLEF; break;
          case SDLK_KP4: objet_actuel=PERSO; break;
          case SDLK_KP5: objet_actuel=BOX_OK; break;          
          } break;    
    }
/* Effacement de l'ecran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 128, 102, 74));
/* Placement des objets a l'ecran */
for (i=0; i<LARGEUR; i++)
{
    for(j=0; j<HAUTEUR; j++)
    {
    pos.x=i * TAILLE;
    pos.y=j * TAILLE;
         switch (carte[i][j]) /* Blit */
         {
         case BRIQUE: SDL_BlitSurface(brique, NULL, ecran, &pos); break;
         case BOX: SDL_BlitSurface(box, NULL, ecran, &pos); break;
         case CLEF: SDL_BlitSurface(clef, NULL, ecran, &pos); break;
         case PERSO: SDL_BlitSurface(perso, NULL, ecran, &pos); perso_creer=1; break;
         case BOX_OK: SDL_BlitSurface(box_ok, NULL, ecran, &pos); break;
         }
    }
}
/* Changement d'image de l'objet actuel */
if (objet_actuel==BRIQUE) objet=brique;
else if (objet_actuel==BOX) objet=box;  
else if (objet_actuel==CLEF) objet=clef;
else if (objet_actuel==PERSO) objet=perso;
else if (objet_actuel==BOX_OK) objet=box_ok;
/* Blit de l'objet actuel sur l'ecran */
SDL_BlitSurface(objet, NULL, ecran, &pos_objet);
/* Mise a jour de l'ecran */
SDL_Flip(ecran);
}
/* Vidage des surfaces de la mémoire */
SDL_FreeSurface(brique);
SDL_FreeSurface(box);
SDL_FreeSurface(clef);
SDL_FreeSurface(perso);
SDL_FreeSurface(box_ok);
// SDL_FreeSurface(edit_instruction); /* Déjà vidé en haut */
SDL_FreeSurface(saved);
}
