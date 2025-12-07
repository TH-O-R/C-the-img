#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Skip all comment lines and leading whitespace until a number or other token
void skip_ppm_comments(FILE *f) {

    int c;
    do {
        c = fgetc(f);
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

    // If we hit a comment, skip the whole line
    while (c == '#') {
        // Skip until end of line
        while ((c = fgetc(f)) != '\n' && c != EOF);
        // Skip any whitespace after the comment line
        do {
            c = fgetc(f);
        } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    }

    // Put back the first non-comment character for the next fscanf()
    if (c != EOF) ungetc(c, f);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s path/to/example.ppm\n", argv[0]);
        return 1;
    }
    
    FILE* image = fopen(argv[1], "rb");
    if(!image) {
        fprintf(stderr, "Failed to read ppm image...\n");
        return 1;
    }
    char magic_number[3];
    int img_w, img_h;
    int maxcol;

    fscanf(image, "%2s", magic_number);
    if(strcmp(magic_number, "P6") != 0 ) {
        fprintf(stderr, "Only supports raw ppm files...\n");
        return 1;
    }
    skip_ppm_comments(image);

    fscanf(image, "%d %d", &img_w, &img_h);
    skip_ppm_comments(image);

    fscanf(image, "%d", &maxcol);
    if(maxcol != 255) {
        fprintf(stderr, "Only supports 255 max value color...\n");
        return 1;
    }
    fgetc(image); // Skip the WhiteSpace after the maxcol val -- According to the .ppm format this is always a thing

    unsigned char *pixels = malloc(img_w * img_h * 3);
    if(!pixels) {
        fprintf(stderr, "Failed to malloc() for pixels\n");
        return 1;
    }

    // We used fread() because it's binary data
    if( fread(pixels, 3, img_h * img_w, image) != img_w*img_h ) {
        fprintf(stderr, "Couldn't get the image pixel data...\n");
        return 1;
    } 

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "FAILED TO INIT SDL2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("PPM viewer", 
                                       SDL_WINDOWPOS_CENTERED,  // X pos
                                       SDL_WINDOWPOS_CENTERED,  // Y pos
                                       img_w, img_h,
                                       0);                      // flags
    if (!win) {
        fprintf(stderr, "FAILED TO CREATE WINDOW: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *winSurface = SDL_GetWindowSurface(win);
    if(!winSurface) {
        fprintf(stderr, "FAILED TO CREATE WINDOW SURFACE: %s\n", SDL_GetError());
        return 1;
    }

    // Fill each pixel of the window surface successivelly
    SDL_LockSurface(winSurface); // Lock the window Surface before editing it's values
    Uint32 *dst = winSurface->pixels;
    int idx; // Keep track of what pixel we're on
    Uint8 pixel_r, pixel_g, pixel_b;
    for(int y = 0; y < img_h; y++) {
        for(int x = 0; x < img_w; x++) {
            idx = (y * img_w + x) * 3; // A Classic
            pixel_r = pixels[idx];
            pixel_g = pixels[idx+1];
            pixel_b = pixels[idx+2];

            dst[y * winSurface->w + x] = SDL_MapRGB(winSurface->format, pixel_r, pixel_g, pixel_b);
        }
    }

    SDL_UnlockSurface(winSurface);
    SDL_UpdateWindowSurface(win);

    //Hack to get window to stay up
    SDL_Event e;
    int quit = 0;
    while(!quit) {
        while( SDL_PollEvent( &e ) ) {
            if( e.type == SDL_QUIT ) quit = 1; 
        } 
        SDL_Delay(100);
    }

    fclose(image);
    free(pixels);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
