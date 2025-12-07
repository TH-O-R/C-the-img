#include <SDL2/SDL.h>
#include <stdio.h>

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
    
    FILE* image = fopen(argv[1], "rb");
    if(!image) {
        fprintf(stderr, "Failed to read ppm image...\n");
        return 1;
    }
    char magic_number[3];
    int img_w, img_h;
    int maxcol;

    fscanf(image, "%2s", magic_number);
    skip_ppm_comments(image);
    fscanf(image, "%d %d", &img_w, &img_h);
    skip_ppm_comments(image);
    fscanf(image, "%d", &maxcol);
    fgetc(image); // Skip the WhiteSpace after the maxcol val -- According to the .ppm format this is always a thing

    unsigned char *pixels = malloc(img_w * img_h * 3);
    if(!pixels) {
        fprintf(stderr, "Failed to malloc() for pixels\n");
        return 1;
    }
    fread(pixels, 3, img_h * img_w, image); // We used fread() because it's binary data

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

    // Fill each pixel of the window successivelly
    SDL_Rect pixel = {0, 0, 1, 1};
    int idx;
    Uint8 pixel_r, pixel_g, pixel_b;
    for(int y = 0; y < img_h; y++) {
        for(int x = 0; x < img_w; x++) {
            idx = (y * img_w + x) * 3;
            pixel_r = pixels[idx];
            pixel_g = pixels[idx+1];
            pixel_b = pixels[idx+2];

            pixel.x = x;
            pixel.y = y;

            SDL_FillRect(winSurface, &pixel, SDL_MapRGB(winSurface->format, pixel_r, pixel_g, pixel_b));
        }
    }

    SDL_UpdateWindowSurface(win);

    //Hack to get window to stay up
    SDL_Event e;
    int quit = 0;
    while( quit == 0 ) {
        while( SDL_PollEvent( &e ) ) {
            if( e.type == SDL_QUIT ) quit = 1; 
        } 
    }

    fclose(image);
    free(pixels);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
