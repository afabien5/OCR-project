#include <assert.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"

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
  return M.M[y * M.width + x];
}

void set(struct Matrix M, int x, int y, int value) {
  assert(in(M, x, y));
  M.M[y * width(M) + x] = value;
}

void highlight(struct Matrix M, int x, int y) {
  assert(in(M, x, y));
  M.M[y * width(M) + x] = get(M, x, y) | 2;
}

int is_blank(int value) {
  return (value & 1) == 0;
}

/*Waiting for a key: */
void ReadText(struct Matrix M, int spacing);
void ReadLine(struct Matrix M, int y1, int y2, int spacing);
void ReadWord(struct Matrix M, int y1 , int y2, int x1, int x2);
void ShrinkCharacter(struct Matrix M, int y1, int y2, int begin, int end);
void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2);

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
                                ReadLine(M, y1, y2, spacing);
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
                        ReadWord(M, y1, y2, x1, x2);
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
                        ShrinkCharacter(M, y1,y2,x1,x2);
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
	ChangeMatrix(M, y1, y2, x1, x2);
}

void ChangeMatrix(struct Matrix M, int y1, int y2, int x1, int x2)
{
        int x, y;
        for (x = x1; x <= x2; ++x) {
                highlight(M, x, y1);
                highlight(M, x, y2);
        }
        for (y = y1; y <= y2; ++y) {
                highlight(M, x1, y);
                highlight(M, x2, y);
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

int main(int argc, char **argv)
{
        if (argc < 2) {
                printf("missing argument\n");
                return 1;
        }
        init_sdl();
        SDL_Surface* image = load_image(argv[1]);
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
	ReadText(M, 2);
	for (int y = 0; y< h; ++y)
        {
                for (int x = 0; x<w; ++x)
                {
			if (get(M, x, y) > 1)
			{
				Uint32 curpixel = SDL_MapRGB(image->format, 255, 0, 0);
				putpixel(image,x,y,curpixel);
			}       
                        
                }
        }
	display_image(image);
        SDL_FreeSurface(image);
  return 0;
}
