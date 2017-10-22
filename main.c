# include <SDL.h>
# include <SDL_image.h>
# include "Traitement/pixel_operations.h"
# include <err.h>
# include "Reseau/neuro.h"

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
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
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

int main(int argc, char **argv)
{
    // argc is the number of parameters
    // argv is the array of parameters
    // ./my_exe my_img.png  -> argc = 2 and argv[1] = "my_img.png"
    // argv[0] is the executable name (here "./my_exe")
  
/*

  if (argc < 2)
  {
    printf("usage: ./main path_to_img\n");
    return -1;
  }

  init_sdl();
  // Load, display original image and wait for key input 
  SDL_Surface *image = load_image(argv[1]);
  display_image(image);

  //wait_for_keypressed();

  // Parse each pixels of image and convert them to grey level
  Uint32 pixel;
  Uint8 r, g, b, rgb_average;
  for (int y = 0; y < image->h; y++)
  {
    for (int x = 0; x < image->w; x++)
    {
      pixel = getpixel(image, x, y);
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      rgb_average = r * 0.3 + g * 0.59 + b * 0.11;
      r = rgb_average;
      g = rgb_average;
      b = rgb_average;
      pixel = SDL_MapRGB(image->format, r, g, b);
      putpixel(image, x, y, pixel);
    }
  }

  // Display and wait...
  display_image(image);
  //wait_for_keypressed();

for (int y = 0; y < image->h; y++)
  {
        for (int x = 0; x < image->w; x++)
        {
                pixel = getpixel(image, x, y);
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if (r>150){r=255;} else r =0;
               // if (g>150){g=255;} else g =0;
               // if (b>150){b=255;} else b =0;
                pixel = SDL_MapRGB(image->format, r, r, r);
                putpixel(image, x, y, pixel);
        }
  }
  display_image(image);
  //wait_for_keypressed();

  SDL_FreeSurface(image);
*/
  printf("%d",argc);
  printf("%s\n", argv[1]);
  Network re = network_init(2,2,1); 

  parcours(re,0.0,1.0);
  update_network(re);
  
  printf("ok\n"); 
  return 0;
}
                                     
