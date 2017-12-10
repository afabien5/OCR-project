#include <assert.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include <stdlib.h>
#include "segmentation.h"

int height(struct Matrix M) {
  return M.height;
}

int width(struct Matrix M) {
  return M.width;
}

int in(struct Matrix M, int x, int y) {
  return x >= 0 && x < width(M) && y >= 0 && y < height(M);
}

int get(struct Matrix M, int x, int y) { 	//des fonctions pratiques sur la struct Matrix
  assert(in(M, x, y));
  return M.M[y * M.width + x]&1;
}

int getcolor(struct Matrix M, int x, int y) {
  assert(in(M, x, y));
  return M.M[y * M.width + x]&(~1);
}

void set(struct Matrix M, int x, int y, int value) {
  assert(in(M, x, y));
  M.M[y * width(M) + x] = value;
}

void highlight(struct Matrix M, int x, int y, int n) {
  assert(in(M, x, y));
  M.M[y * width(M) + x] |= n;
}

int is_blank(int value) {
  return (value & 1) == 0;
}

double min(double a, double b)
{
	return (a<b) ? a : b;
}
double max(double a, double b)		//min et max, tjs utile
{
	return (a>b) ? a : b;
}
struct Matrix resize(struct Matrix M, int y1, int y2, int x1, int x2, int newsize);
void ReadLine(struct Matrix M, int y1, int y2, int spacing, struct Callbacks callbacks);
void ReadWord(struct Matrix M, int y1 , int y2, int x1, int x2, struct Callbacks callbacks);
void ShrinkCharacter(struct Matrix M, int y1, int y2, int begin, int end, struct Callbacks callbacks);
void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2, int n);

/*ReadText parcours la matrice et détecte les lignes, définies comme étant un espace avec des pixels noirs contenu entre deux lignes de pixels blanc uniquements*/
//Callback contient toutes fonctions à appliquer une fois qu'on a nos lignes, mots, lettres etc. (voir main.c pour les voir) 
void ReadText (struct Matrix M, int spacing, struct Callbacks callbacks)
{
	assert(spacing > 0); //au cas ou hein

        for (int y=0; y<height(M) ; ++y)
        {
                        int x=0; 
                        while (x < width(M) && is_blank(get(M, x, y)))
                        {
                                x++;
                        }
                        if (x<width(M)) //ca veut dire qu'on est tombé sur un pixel noir
                        {
                                int y1 = y; //haut de la ligne
                                while (++y < height(M))
                                {
                                        x = 0;
                                        while (x < width(M) && is_blank(get(M, x, y)))
                                        {
                                                x++;
                                        }
                                        if (x == width(M)) //une ligne avec que du blanc
                                                break;
                                }
                                int y2 = y; //la ligne que de blanc
                                ReadLine(M, y1, y2-1, spacing, callbacks); //y2-1 pour enlever ligne blanche
				ChangeMatrix(M, y1-3, y2+2, 0, width(M)-1, 8);//ajout marqueur couleur 
                        }
        }
}
//meme choses mais parcours gauche à droite
void ReadLine(struct Matrix M, int y1, int y2, int spacing, struct Callbacks callbacks)
{
        int top = y1;
	int bottom = y2;
	callbacks.start_line(M, 0, width(M)-1, top, bottom); //fn qu il devrait faire dès qu il trouve une ligne, en l occurrence rien.

        for (int x = 0; x<width(M) ; ++x)
        {
                int y = y1;
                while (y<y2 && is_blank(get(M, x, y)))
                        y++;
                if (y<y2) //on a trouvé une colonne avec du noir, donc début mot
                {
                        int currentspace = 0;
                        int x1 = x;
                        while (++x<width(M))
                        {
                                y = y1;
                                while (y<y2 && is_blank(get(M, x, y)))
                                        y++;
                                if (y <y2)
                                        currentspace = 0;
                                else    
                                        ++currentspace; 
                                if (currentspace == spacing) //on trouve un nombre suffisant de colonnes blanches donc on a fini le mot
                                        break;
                        }
                        int x2 = x - currentspace; //pas besoin de -1, on enleve deja colonne blanche
                        ReadWord(M, y1, y2, x1, x2, callbacks);
			ChangeMatrix(M, y1-2, y2+2, x1-2, x2+2, 4);//marqueur couleur	
                }
        }

	callbacks.stop_line(M, 0, width(M)-1, top, bottom); //fn des qu il fini la ligne, ici \n
}
//meme algo que pour les mots mais pas de spacing, juste des qu il trouve un espace.
void ReadWord(struct Matrix M, int y1, int y2, int x1, int x2, struct Callbacks callbacks)
{
	int left = x1;
	int right = x2;
	int top = y1;
        int bottom = y2;
	callbacks.start_word(M, left, right, top, bottom); //fait rien mais pourrait
        for(int x =x1; x<x2; ++x)
        {
                int y = y1;
                while (y<y2 && is_blank(get(M, x, y)))
                        y++;
                if (y<y2)
                {
                        int x1 = x;
                        while (++x<x2)
                        {
                                y = y1;
                                while(y<y2 && is_blank(get(M, x, y)))
                                        y++;
                                if (y ==y2)
                                        break;
                        }
                        int x2 = x;
                        ShrinkCharacter(M, y1,y2,x1,x2-1, callbacks);
                }
        }
	callbacks.stop_word(M, left, right, top, bottom); //print un espace
}
//réduit les coords ds lesquelles est contenu la lettre au strict minimum
void ShrinkCharacter(struct Matrix M, int y1, int y2, int x1, int x2, struct Callbacks callbacks)
{
	int x;
        int y = y1;
        while (y<y2)
        {
                x = x1;
                while (x <x2 && is_blank(get(M, x, y)))
                        ++x;
                if (x<x2)
                {
                        y1 = y;
                        break;
                }
                y++;

        }
        y = y2;
        while (y>y1)
        {
                x = x1;
                while (x<x2 && is_blank(get(M, x, y)))
                        ++x;
                if (x<x2)
                {
                        y2 = y;
                        break;
                }
                --y;
        }
        x = x1;
        while (x<x2)
        {
                y = y1;
                while (y<=y2 && is_blank(get(M, x, y)))
                        ++y;
                if (y<=y2)
                {
                        x1 = x;
                        break;
                }
                ++x;
        }
        x = x2;
        while (x>x1)
        {
                y = y1;
                while(y<=y2 && is_blank(get(M, x, y)))
                        ++y;
                if (y<=y2)
                {
                        x2 = x;
                        break;
                }
                --x;
        }

	struct Matrix resized = resize(M, y1,y2,x1,x2, 16); //appel resize qui créé matrice 16*16
        callbacks.start_letter(resized, 0, width(resized)-1, 0, height(resized)-1);//appelle fn reconnaissance de la lettre à partir de matrice 16*16, qui print la lettre une fois reconnue.
        callbacks.stop_letter(resized, 0, width(resized)-1, 0, height(resized)-1);
	free(resized.M); // libère mémoire matrice 16*16
	ChangeMatrix(M, y1-1, y2+1, x1-1, x2+1, 2); //marqueur de couleur
}

void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2, int n)
{
	if (y1 < 0)
		y1=0;
	if (x1 < 0)
		x1=0;
	if (y2 >= height(M))
		y2 = height(M)-1;
	if (x2 >= width(M))
		x2 = width(M)-1;
	if (y2 < 0)
		y2=0;
	if (x2 < 0)
		x2=0;
        int x, y;
        for (x = x1; x <= x2; ++x) {
                highlight(M, x, y1, n);
                highlight(M, x, y2, n);
        }
        for (y = y1; y <= y2; ++y) {
                highlight(M, x1, y, n);
                highlight(M, x2, y, n);
        }
}

void PrintMatrix(struct Matrix M) //à servi à vérifier que le resize conserve la forme ^^
{
	 for (int i = 0; i<height(M); ++i)
        {
                for (int j = 0; j<width(M); ++j)
                {
                        printf("%i",get(M, j, i));
                }
                printf("\n");
        }
}
/*en gros le resize va créer une matrice 16*16, et on va la parcourir. Pour chaque case, on chope ses coords dans la Matrice d origine et on calcul pour chaque case de cette derniere l'intersection avec la case de cette derniere.
On fait une moyenne et on obtient un pixel noir ou blanc. Formule E (val_pixel * air) / E airs (E étant epsilonne)*/
struct Matrix resize(struct Matrix M,  int y1, int y2, int x1, int x2, int newsize)
{
	struct Matrix NewMat;
	NewMat.width = newsize;
	NewMat.height = newsize;
	NewMat.M = calloc(newsize*newsize, sizeof(int));
	int L = x2-x1+1;
	int H = y2-y1+1;
	double leftside;
	double rightside;
	double top;
	double bottom;
	double A;
	double A_sum;
	double A_val;
	double A_val_sum;
	double ratio;
	for (int i = 0; i<newsize; ++i)
	{
		for (int j = 0 ; j<newsize; ++j)
		{	A_sum = 0;
			A_val_sum = 0;
			double mapped_left = ((double)(i*L))/newsize; //coords coté gauche equivalent de mat 16*16 dans matrice originale
			double mapped_right = ((double)((i+1)*L))/newsize; // droit
			double mapped_top = ((double)(j*H))/newsize; //haut
			double mapped_bottom = ((double)((j+1)*H))/newsize;//bas
			for (int k = 0; k <= L; ++k)
			{
				for (int l = 0; l<= H; ++l)
				{	A=0;
					A_val =0;
					leftside = max(mapped_left, k);
					rightside = min(mapped_right,k+1);
				 	top = max(mapped_top, l);
					bottom = min(mapped_bottom, l+1);
					if (leftside<rightside && bottom > top)
					{
						A = (rightside - leftside) * (bottom-top); //air d intersection
						A_val = A * get(M, x1 + k, y1 + l);
						A_val_sum += A_val;
						A_sum += A;	
					}
				}
			}
			ratio = A_val_sum/A_sum; //formule magique
			assert(ratio<=1);
			if (ratio<0.5)
				set(NewMat, i, j, 0); //le pixel est blanc
			else
				set(NewMat, i, j, 1); //le pixel est noir	
		}
	}
	return NewMat;
}
