#include <assert.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include <stdlib.h>
#include "segmentation.h"

void do_nothing(struct Matrix M, int x1, int x2, int y1, int y2) {
  (void)M;
  (void)x1;
  (void)x2;
  (void)y1;
  (void)y2;
}

void output_newline(struct Matrix M, int x1, int x2, int y1, int y2) {
  (void)M;
  (void)x1;
  (void)x2;
  (void)y1;
  (void)y2;
	printf("\n");
}

void output_space(struct Matrix M, int x1, int x2, int y1, int y2) {
  (void)M;
  (void)x1;
  (void)x2;
  (void)y1;
  (void)y2;
        printf(" ");
}

void dump_matrix(struct Matrix M, int x1, int x2, int y1, int y2) {
  (void)x1;
  (void)x2;
  (void)y1;
  (void)y2;
	PrintMatrix(M);
}

/*Struct qui contient des fonctions et qui sera dans les parametre de ReadTexte et des fonctions que ce dernier appelle.(voir le segmentation.h)
En gros, par exemple la ca contient output_newline en stop_line(struct M, int x1, int x2, int y1, int y2), comme ca je l appelle à chaque fois que j ai fini de détecter une ligne et ca renvoi à la ligne ds la console.
Comme ca on peut juste mettre la fn qui prend une Matrice 16*16 et devine la lettre à la place d'un des deux derniers dp_nothing ci dessous (la fonction détecte la lettre et doit la print sans retour à la ligne)*/
struct Callbacks debug_callbacks = {
	do_nothing, output_newline,
	do_nothing, output_space,
	dump_matrix, do_nothing
};

/*Bon là on a tout le SDL en vrac, pas intéressant*/

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}

SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n", img->w, img->h, SDL_GetError());

  }

  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());

  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);

  // wait for a key
  wait_for_keypressed();

  // return the screen for further uses
  return screen;
}

/*on passe au main t'as vu*/

int main(int argc, char **argv)
{
	/*arg[1] c'est le path pour l'image, l'arg2 c'est le nombre de pixel entre les mots, par exmple 20 en spacing c est bien pour Projet.png*/
        if (argc < 3) {
                printf("missing argument\n");
                return 1;
        }
/*On créé 3 surfaces SDL, meme si la premiere sert un peut à rien, le reste sert à créer les 3 images de segmentation (le nom est assez obvious)*/
        init_sdl();
        SDL_Surface* image = load_image(argv[1]);
				SDL_Surface* imageLines = load_image(argv[1]);
				SDL_Surface* imageWords = load_image(argv[1]);
				SDL_Surface* imageLetters = load_image(argv[1]);
	int h = image->h;
	int w = image->w;
	printf("%i",h);
	printf("%i",w);
	int Mat[h*w];
        struct Matrix M = { Mat, w, h };
/*On créé une matrice avec des 0 de la taille de l'image avec 0 comme pixels blancs et 1 pour pixels noirs*/
        for (int y = 0; y< h; ++y)
        {
                for (int x = 0; x<w; ++x)
                {	
                        Uint32 curpixel = getpixel(image, x,y);
      			Uint8 r,g,b;
                        SDL_GetRGB(curpixel, image->format, &r, &g, &b);
                        set(M, x, y, (r + g + b < 100) ? 1 : 0);
                }
        }
	int spacing = atoi(argv[2]);
	ReadText(M, spacing, debug_callbacks); //appel de ReadText, pour suivre, aller dans segmentation.c

/*Ok on a tout parcouru etc. maintenant on veut encadrer dans nos 3 surfaces créées précédemment pour faire joli. Pour cela on a modifié les bits après le 1 ou 0 tt a droite permettant de savoir si le pixel est noir ou blanc. Les int ds la matrices seront donc de la forme 1??? (>=8) si c est un marqueur ligne (bleu), ?1?? (>=4) si marqueur mot (vert) et ??1? (>=2) si marqueur lettre.
J'ai essayer de pas mettresur les memes cases de la matrice pour eviter de se retrouver avec par exemple 110? qui serait un pixel noir ou blanc (d ou le ?) avec un marqueur bleu et vert, meme si d'apres mon implémentation ca devrait marcher*/ 
	for (int y = 0; y< h; ++y)
        {
                for (int x = 0; x<w; ++x)
                {
			if (getcolor(M, x, y) >=8) //if y a un marqueur ligne (bleu)
			{
				Uint32 curpixel = SDL_MapRGB(imageLines->format, 0, 0, 255);
				putpixel(imageLines,x,y,curpixel);
				M.M[y*width(M)+x] -= 8; //on enleve 8 pour les prchains if comme ca si on a par exemple 1101, ca devient 0101 et donc on a enlevé le marqueur bleu
				
			}
			if (getcolor(M, x, y) >=4 ) //if y a marqueur mot (vert), on remarque que si y avait un marqueur bleu mais pas vert, le -8 avant rend le nombre <4 et on rentre pas ds le if
      			{
        			Uint32 curpixel = SDL_MapRGB(imageWords->format, 0, 255, 0);
       	 			putpixel(imageWords,x,y,curpixel);
				M.M[y*width(M)+x] -=4;//meme chose on enleve 4, 0101 devient 0001 (plus marqueur vert)
      			}
			if (getcolor(M, x, y) >= 2)//if marqueur lettre (rouge)
      			{
        			Uint32 curpixel = SDL_MapRGB(imageLetters->format, 255, 0, 0);
        			putpixel(imageLetters,x,y,curpixel);
				M.M[y*width(M)+x] -=2; //pas vraiment utile mais comme ca on revient à un pixel juste blanc ou noir sans marqueur
      			}                       
                }
        }
	SDL_SaveBMP( imageLines, "imageLines.bmp" );
	SDL_SaveBMP( imageWords, "imageWords.bmp" ); //sauvegarde les images
	SDL_SaveBMP( imageLetters, "imageLetters.bmp" ); 
  SDL_FreeSurface(imageLines); 
  SDL_FreeSurface(imageWords);//libère les surfaces SDL
  SDL_FreeSurface(imageLetters);
  SDL_FreeSurface(image);
  return 0;
}
