#include <assert.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include <stdlib.h>
#include <segmentation.h>
struct Matrix {
  int *M;
  int width;
  int height;
};

int height(struct Matrix M) {
  return M.height;
}

int width(struct Matrix M) {
  return M.width;
}

int in(struct Matrix M, int x, int y) {
  return x >= 0 && x < width(M) && y >= 0 && y < height(M);
}

int get(struct Matrix M, int x, int y) {
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
double max(double a, double b)
{
	return (a>b) ? a : b;
}
void PrintMatrix(struct Matrix M);
struct Matrix resize(struct Matrix M, int y1, int y2, int x1, int x2, int newsize);
void ReadText(struct Matrix M, int spacing);
void ReadLine(struct Matrix M, int y1, int y2, int spacing);
void ReadWord(struct Matrix M, int y1 , int y2, int x1, int x2);
void ShrinkCharacter(struct Matrix M, int y1, int y2, int begin, int end);
void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2, int n);

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

void ReadText (struct Matrix M, int spacing)
{
        for (int y=0; y<height(M) ; ++y)
        {
                        int x=0; 
                        while (x < width(M) && is_blank(get(M, x, y)))
                        {
                                x++;
                        }
                        if (x<width(M))
                        {
                                int y1 = y;
                                while (++y < height(M))
                                {
                                        x = 0;
                                        while (x < width(M) && is_blank(get(M, x, y)))
                                        {
                                                x++;
                                        }
                                        if (x == width(M))
                                                break;
                                }
                                int y2 = y;
                                ReadLine(M, y1, y2-1, spacing);
				ChangeMatrix(M, y1-3, y2+2, 0, width(M)-1, 8);
                        }
        }
}

void ReadLine(struct Matrix M, int y1, int y2, int spacing)
{
        for (int x = 0; x<width(M) ; ++x)
        {
                int y = y1;
                while (y<y2 && is_blank(get(M, x, y)))
                        y++;
                if (y<y2)
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
                                if (currentspace == spacing)
                                        break;
                        }
                        int x2 = x-spacing+1; 
                        ReadWord(M, y1, y2, x1, x2-1);
			ChangeMatrix(M, y1-2, y2+2, x1-2, x2+1, 4);	
                }
        }
}

void ReadWord(struct Matrix M, int y1, int y2, int x1, int x2)
{
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
                        ShrinkCharacter(M, y1,y2,x1,x2-1);
                }
        }
}

void ShrinkCharacter(struct Matrix M, int y1, int y2, int x1, int x2)
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
	ChangeMatrix(M, y1-1, y2+1, x1-1, x2+1, 2);
	PrintMatrix(resize(M, y1,y2,x1,x2, 16));

}

void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2, int n)
{
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

void PrintMatrix(struct Matrix M)
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
			double mapped_left = ((double)(i*L))/newsize;
			double mapped_right = ((double)((i+1)*L))/newsize;
			double mapped_top = ((double)(j*H))/newsize;
			double mapped_bottom = ((double)((j+1)*H))/newsize;
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
						A = (rightside - leftside) * (bottom-top);
						A_val = A * get(M, x1 + k, y1 + l);
						A_val_sum += A_val;
						A_sum += A;	
					}
				}
			}
			ratio = A_val_sum/A_sum;
			assert(ratio<=1);
			if (ratio<0.5)
				set(NewMat, i, j, 0);
			else
				set(NewMat, i, j, 1);	
		}
	}
	return NewMat;
}

/*
SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
    if(!Surface || !Width || !Height)
        return 0;

    SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

    double    _stretch_factor_x = ((double)(Width)  / (double)(Surface->w)),
        _stretch_factor_y = ((double)(Height) / (double)(Surface->h));

    for(Sint32 y = 0; y < Surface->h; y++)
        for(Sint32 x = 0; x < Surface->w; x++)
            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    putpixel(_ret ,  (int)(_stretch_factor_x * x) + o_x , (int)(_stretch_factor_y * y) + o_y , getpixel(Surface, x, y));

    return _ret;
}
*/
int main(int argc, char **argv)
{
        if (argc < 3) {
                printf("missing argument\n");
                return 1;
        }
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
	int spacing = *argv[2];
	ReadText(M, spacing);
	for (int y = 0; y< h; ++y)
        {
                for (int x = 0; x<w; ++x)
                {
			if (getcolor(M, x, y) >=8)
			{
				Uint32 curpixel = SDL_MapRGB(imageLines->format, 0, 0, 255);
				putpixel(imageLines,x,y,curpixel);
				M.M[y*width(M)+x] -= 8;
				
			}
			if (getcolor(M, x, y) >=4 )
      			{
        			Uint32 curpixel = SDL_MapRGB(imageWords->format, 0, 255, 0);
       	 			putpixel(imageWords,x,y,curpixel);
				M.M[y*width(M)+x] -=4;
      			}
			if (getcolor(M, x, y) >= 2)
      			{
        			Uint32 curpixel = SDL_MapRGB(imageLetters->format, 255, 0, 0);
        			putpixel(imageLetters,x,y,curpixel);
				M.M[y*width(M)+x] -=2;
      			}                       
                }
        }
	SDL_SaveBMP( imageLines, "imageLines.bmp" );
	SDL_SaveBMP( imageWords, "imageWords.bmp" );
	SDL_SaveBMP( imageLetters, "imageLetters.bmp" ); 
  SDL_FreeSurface(imageLines);
  SDL_FreeSurface(imageWords);
  SDL_FreeSurface(imageLetters);
  SDL_FreeSurface(image);
  return 0;
}
