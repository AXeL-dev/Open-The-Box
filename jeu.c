#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "const.h"
#include "jeu.h"
#include "level.h"

/* Fonction du jeu */
void play(SDL_Surface *ecran)
{
FILE* c_level=NULL;
SDL_Surface *perso[4]={NULL};
SDL_Surface *brique=NULL, *box=NULL, *box_ok=NULL, *clef=NULL, *statut_perso=NULL;
SDL_Surface  *win=NULL, *game_instruction=NULL, *end=NULL, *save=NULL;
SDL_Rect pos, pos_perso, pos_win, pos_g_instruction, pos_end, pos_save;
SDL_Event event;
bool continuer=1, clef_r=0, c_g_instruction=1;
int i=0, j=0, save_i=0, save_j=0, carte[LARGEUR][HAUTEUR]={0}, level;
/* Détection du (current/actuel)level */
c_level=fopen("ressources\\c_level.lvl", "r");
if (c_level==NULL)
{
fprintf(stderr, "Erreur lors du chargement du level actuel.\n");
exit(EXIT_FAILURE);                  
}
fscanf(c_level, "%d", &level);
fclose(c_level);
/* Les sprites du jeu */
brique=IMG_Load("ressources\\brique.png");
box=IMG_Load("ressources\\box.png");
box_ok=IMG_Load("ressources\\box_o.png");
clef=IMG_Load("ressources\\clef.png");
perso[HAUT]=IMG_Load("ressources\\perso_h.png");
perso[BAS]=IMG_Load("ressources\\perso_b.png");
perso[GAUCHE]=IMG_Load("ressources\\perso_g.png");
perso[DROITE]=IMG_Load("ressources\\perso_d.png");
statut_perso=perso[BAS];
/* Activation de la transparance des 4 faces du perso */
SDL_SetColorKey(perso[HAUT], SDL_SRCCOLORKEY, SDL_MapRGB(perso[HAUT]->format, 255, 255, 255));
SDL_SetColorKey(perso[BAS], SDL_SRCCOLORKEY, SDL_MapRGB(perso[BAS]->format, 255, 255, 255));
SDL_SetColorKey(perso[GAUCHE], SDL_SRCCOLORKEY, SDL_MapRGB(perso[GAUCHE]->format, 255, 255, 255));
SDL_SetColorKey(perso[DROITE], SDL_SRCCOLORKEY, SDL_MapRGB(perso[DROITE]->format, 255, 255, 255));
/* Les sprites d'instructions */
save=IMG_Load("ressources\\saved.png");
win=IMG_Load("ressources\\win.png");
game_instruction=IMG_Load("ressources\\game_instruction.png");
end=IMG_Load("ressources\\end.png");
pos_g_instruction.x=0;
pos_g_instruction.y=0;
SDL_BlitSurface(game_instruction, NULL, ecran, &pos_g_instruction);
/* Mise a jour de l'ecran apres blit des instructions */
SDL_Flip(ecran);
/* Attente d'appui sur une touche pour commencer le jeu */
while (c_g_instruction)
{
SDL_WaitEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT: c_g_instruction=0; continuer=0; break;
    case SDL_KEYDOWN: 
         switch (event.key.keysym.sym)
         {
         case SDLK_ESCAPE: c_g_instruction=0; continuer=0; break;
         default: c_g_instruction=0; SDL_FreeSurface(game_instruction); break;
         } break;
    }
}
/* Chargement de la carte du jeu */
if (!charger_niveau(carte, level)) /* S'il y'a une erreur lors du chargement de la carte */
{
fprintf(stderr, "Erreur lors du chargement de la carte.\n");
exit(EXIT_FAILURE);
}
/* Position du perso (recherche) */
for (i=0; i<LARGEUR; i++)
{
    for(j=0; j<HAUTEUR; j++)
    {
        if (carte[i][j]==PERSO) /* ça dépond du fichier level.lvl */
        {
        pos_perso.x=i;
        pos_perso.y=j;
        save_i=i;
        save_j=j;
        carte[i][j]=VIDE;
        }
    }
}
/* Répétition des touches */
SDL_EnableKeyRepeat(100, 100);
/* Gestion d'évenements */
while (continuer)
{
SDL_WaitEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT: continuer=0; break;
    case SDL_KEYDOWN:
         switch (event.key.keysym.sym)
         {
         case SDLK_ESCAPE: continuer=0; break;
         case SDLK_r: charger_niveau(carte, level); statut_perso=perso[BAS]; pos_perso.x=save_i; pos_perso.y=save_j; break;
         case SDLK_s:
         /* Sauvegarde */
         c_level=fopen("ressources\\c_level.lvl", "w");
         if (c_level==NULL) /* Si le fichier n'existe pas */
         {
         fprintf(stderr, "Erreur lors de la sauvegarde.\n");
         exit(EXIT_FAILURE);                  
         }
         fprintf(c_level, "%d", level);
         fclose(c_level);
         /* Placement au milieu + Blit */
          pos_save.x=(ecran->w/2)-(save->w/2);
          pos_save.y=(ecran->h/2)-(save->h/2);
          SDL_BlitSurface(save, NULL, ecran, &pos_save); 
          /* Mise a jour de l'ecran */
          SDL_Flip(ecran);
          /* Initialisation */
          c_g_instruction=1;
              while (c_g_instruction)
              {
              SDL_WaitEvent(&event);
                  switch(event.type)
                  {
                  case SDL_QUIT: c_g_instruction=0; continuer=0; break;
                  case SDL_KEYDOWN: 
                       switch (event.key.keysym.sym)
                       { 
                       case SDLK_ESCAPE: c_g_instruction=0; continuer=0; break;
                       default: c_g_instruction=0; break;
                       } break;
                  }
               } 
         break;
         case SDLK_UP: statut_perso=perso[HAUT]; deplacer(carte, &pos_perso, HAUT); break;
         case SDLK_DOWN: statut_perso=perso[BAS]; deplacer(carte, &pos_perso, BAS); break;
         case SDLK_LEFT: statut_perso=perso[GAUCHE]; deplacer(carte, &pos_perso, GAUCHE); break;
         case SDLK_RIGHT: statut_perso=perso[DROITE]; deplacer(carte, &pos_perso, DROITE); break;
         } break;
    }
/* Effacement de l'ecran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 128, 102, 74));
clef_r=0; /* Initialisation */
/* Placement des objets a l'ecran */
for (i=0; i<LARGEUR; i++)
{
    for (j=0; j<HAUTEUR; j++)
    {
    pos.x=i * TAILLE;
    pos.y=j * TAILLE;
        switch (carte[i][j]) /* Blit (pixel par pixel) */
        {
        case BRIQUE: SDL_BlitSurface(brique, NULL, ecran, &pos); break;
        case BOX: SDL_BlitSurface(box, NULL, ecran, &pos); break;
        case BOX_OK: SDL_BlitSurface(box_ok, NULL, ecran, &pos); break;
        case CLEF: SDL_BlitSurface(clef, NULL, ecran, &pos); clef_r=1; break;
        }
    }
}
if (!clef_r) /* Si la partie est fini */
{
/* Positionnement du message au milieu */
pos_win.x=(ecran->w/2)-(win->w/2);
pos_win.y=(ecran->h/2)-(win->h/2);
/* Blit */
SDL_BlitSurface(win, NULL, ecran, &pos_win); 
/* Mise a jour de l'ecran */
SDL_Flip(ecran);
/* Initialisation */
c_g_instruction=1;
/* Attente d'appuie sur une touche */
  while (c_g_instruction)
  {
  SDL_WaitEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT: c_g_instruction=0; continuer=0; break;
    case SDL_KEYDOWN: 
         switch (event.key.keysym.sym)
         {
         default: c_g_instruction=0; level++; /* Passage au prochain level */
            if(!charger_niveau(carte, level)) /* Si tout les levels fini */
            {
            /* Rénitialisation du jeu */
            reset(ecran, 0);
            /* Positionnement du message au milieu */
            pos_end.x=(ecran->w/2)-(end->w/2);
            pos_end.y=(ecran->h/2)-(end->h/2);
            SDL_BlitSurface(end, NULL, ecran, &pos_end); 
            /* Mise a jour de l'ecran */
            SDL_Flip(ecran);
            /* Rénitialisation */
            c_g_instruction=1;
               while (c_g_instruction)
               {
               SDL_WaitEvent(&event);
                  switch(event.type)
                  {
                  case SDL_QUIT: c_g_instruction=0; continuer=0; break;
                  case SDL_KEYDOWN: 
                       switch (event.key.keysym.sym)
                       { 
                       default: c_g_instruction=0; continuer=0; break;
                       } break;
                  }
               }
            }
            else
            {
            /* Repositionnement du perso (recherche) */
               for (i=0; i<LARGEUR; i++)
               {
                  for(j=0; j<HAUTEUR; j++)
                  {
                    if (carte[i][j]==PERSO) /* ça dépond du fichier level.lvl */
                    {
                    pos_perso.x=i;
                    pos_perso.y=j;
                    save_i=i;
                    save_j=j;
                    carte[i][j]=VIDE;
                    }
                  }
               }
            /* Réaffichage de la face bas du perso */
            statut_perso=perso[BAS];
            }    
         break;
         } break;
    }
  }            
}
/* Placement du perso */
pos.x=pos_perso.x * TAILLE; /* Pour avancé de TAILLE(=32) pixel */
pos.y=pos_perso.y * TAILLE;
SDL_BlitSurface(statut_perso, NULL, ecran, &pos);
/* Mise a jour de l'ecran */
SDL_Flip(ecran);
}
/* Désactivation de la répétition des touches */
SDL_EnableKeyRepeat(0,0);
/* Vidage des surfaces de la mémoire */
SDL_FreeSurface(brique);
SDL_FreeSurface(box);
SDL_FreeSurface(box_ok);
SDL_FreeSurface(clef);
for (i=0; i<4; i++) SDL_FreeSurface(perso[i]);
SDL_FreeSurface(win);
// SDL_FreeSurface(game_intruction); /* Déjà vidé en haut */
SDL_FreeSurface(end);
SDL_FreeSurface(save);
}


/* Fonction de déplacement du perso */
void deplacer(int carte[][HAUTEUR], SDL_Rect *pos, int direction)
{
/* Gestion des collisions */
switch (direction)
{
case HAUT:
     if (pos->y - 1 < 0) break; /* Si le perso dépasse l'ecran */
     if (carte[pos->x][pos->y - 1]==BRIQUE) break; /* S'il y'a une brique */
     /* Pour pousser le coffre(box) on vérifie : */
     if ( (carte[pos->x][pos->y - 1]==BOX || carte[pos->x][pos->y - 1]==BOX_OK) && (pos->y - 2 < 0 || carte[pos->x][pos->y - 2]==BRIQUE || carte[pos->x][pos->y - 2]==BOX || carte[pos->x][pos->y - 2]==BOX_OK) ) break;
deplacer_box(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2]);
pos->y--;
break;
case BAS:
     if (pos->y + 1 >= HAUTEUR) break;
     if (carte[pos->x][pos->y + 1]==BRIQUE) break;
     if ( (carte[pos->x][pos->y + 1]==BOX || carte[pos->x][pos->y + 1]==BOX_OK) && (pos->y + 2 >= HAUTEUR || carte[pos->x][pos->y + 2]==BRIQUE || carte[pos->x][pos->y + 2]==BOX || carte[pos->x][pos->y + 2]==BOX_OK) ) break;
deplacer_box(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2]);
pos->y++;
break;
case GAUCHE:
     if (pos->x - 1 < 0) break;
     if (carte[pos->x - 1][pos->y]==BRIQUE) break;
     if ( (carte[pos->x - 1][pos->y]==BOX || carte[pos->x - 1][pos->y]==BOX_OK) && (pos->x - 2 < 0 || carte[pos->x - 2][pos->y]==BRIQUE || carte[pos->x - 2][pos->y]==BOX || carte[pos->x - 2][pos->y]==BOX_OK) ) break;
deplacer_box(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y]);
pos->x--;
break;
case DROITE:
     if (pos->x + 1 >= LARGEUR) break;
     if (carte[pos->x + 1][pos->y]==BRIQUE) break;
     if ( (carte[pos->x + 1][pos->y]==BOX || carte[pos->x + 1][pos->y]==BOX_OK) && (pos->x + 2 >= LARGEUR || carte[pos->x + 2][pos->y]==BRIQUE || carte[pos->x + 2][pos->y]==BOX || carte[pos->x + 2][pos->y]==BOX_OK) ) break;
deplacer_box(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y]);
pos->x++;
break;
}
}


/* Fonction de déplacement des boxs(coffres) */
void deplacer_box(int *p_case, int *d_case)
{
if (*p_case==BOX || *p_case==BOX_OK)
{
      if (*d_case==CLEF) *d_case=BOX_OK; /* Si la 2ème case(qu'on poussera dedans) est une clef on met la box_ok */
      else *d_case=BOX; /* Si la 2ème case n'est pas une clef on met la box quand pousse alors */
      if (*p_case==BOX_OK) *p_case=CLEF; /* Si la 1ère case qu'on pousse est une box_ok on remet la clef */
      else *p_case=VIDE; /* Si la case qu'on pousse est une box on met la case derrière elle vide */
}
}
