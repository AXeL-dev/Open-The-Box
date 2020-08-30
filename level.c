#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "const.h"
#include "level.h"

/* Fonction qui charge le niveau */
int charger_niveau(int niveau[][HAUTEUR], int level)
{
FILE* fichier=NULL;
char ligne_fichier[LARGEUR * HAUTEUR + 1]={0};
int i=0, j=0;
fichier=fopen("ressources\\levels.lvl", "r");
if (fichier==NULL) return 0;
rewind (fichier);
for (i=0; i<level; i++) 
if (fread (ligne_fichier, 1, LARGEUR * HAUTEUR + 1, fichier)==0) return 0; /* Si on arrive a la fin du fichier */
ligne_fichier[LARGEUR * HAUTEUR + 1]='\0';
for (i=0; i<LARGEUR; i++)
{
    for (j=0; j<HAUTEUR; j++)
    {
        switch (ligne_fichier[(i * LARGEUR) + j])
        {
        case '0': niveau[j][i]=VIDE; break;
        case '1': niveau[j][i]=BRIQUE; break;
        case '2': niveau[j][i]=BOX; break;
        case '3': niveau[j][i]=CLEF; break;
        case '4': niveau[j][i]=PERSO; break;
        } 
    }    
} 
fclose(fichier);
return 1;   
}


/* Fontion qui sauvegarde le niveau */
int sauvegarder_niveau(int niveau[][HAUTEUR])
{
FILE* fichier=NULL;
FILE* f_origine=NULL;
int i=0, j=0;
char ligne_fichier[LARGEUR * HAUTEUR + 1]={0};
fichier=fopen("ressources\\levels_m.lvl", "w");
if (fichier==NULL) return 0;
for (i=0; i<LARGEUR; i++)
{
    for (j=0; j<HAUTEUR; j++)
    {
    fprintf(fichier, "%d", niveau[j][i]);    
    }
}
fprintf(fichier, "\n"); /* On revien a la ligne */
f_origine=fopen("ressources\\levels.lvl", "r");
for (i=0; i<20; i++)  /* On a 20 level dans le jeu ! */
{
if (fread (ligne_fichier, 1, LARGEUR * HAUTEUR + 1, f_origine)==0) break;
ligne_fichier[LARGEUR * HAUTEUR + 1]='\0';
if (i>0) fprintf(fichier, "%s", ligne_fichier); /* Si on dépasse la ligne du 1er level */
// fwrite(ligne_fichier, 1, LARGEUR * HAUTEUR + 1, fichier);
}
fclose(f_origine);
fclose(fichier);
remove("ressources\\levels.lvl"); /* Suppression du fichier d'origine */
rename("ressources\\levels_m.lvl", "ressources\\levels.lvl"); /* On renomme le fichier */
return 1;
}


/* Fonction qui rénitialise le jeu */
void reset(SDL_Surface *ecran, bool afficher)
{
FILE* c_level=NULL;
int level=1;
/* Reset */
c_level=fopen("ressources\\c_level.lvl", "w");
if (c_level==NULL) /* Meme si ce cas arrive peu */
{
fprintf(stderr, "Erreur lors de la rénitialisation.\n");
exit(EXIT_FAILURE);                  
}
fprintf(c_level, "%d", level);
fclose(c_level);
if (afficher) /* Si vrai(=1) */
{
/* Initialisation */
SDL_Surface *reset=NULL;
SDL_Rect pos_reset;
SDL_Event event;
bool r_level=1;
/* Sprite */
reset=IMG_Load("ressources\\reset.png");
/* Placement au milieu + Blit */
pos_reset.x=(ecran->w/2)-(reset->w/2);
pos_reset.y=(ecran->h/2)-(reset->h/2);
SDL_BlitSurface(reset, NULL, ecran, &pos_reset); 
/* Mise a jour de l'ecran */
SDL_Flip(ecran);
/* Gestion d'évenements */
   while (r_level)
   {
   SDL_WaitEvent(&event);
      switch(event.type)
      {
      case SDL_QUIT: r_level=0; break;
      case SDL_KEYDOWN: 
         switch (event.key.keysym.sym)
         { 
         default: r_level=0; break;
         } break;
      }
   }
/*  Vidage de la mémoire */
SDL_FreeSurface(reset);
}
}
