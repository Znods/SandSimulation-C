#include "headers/common.h"
#include "headers/structs.h"
#include "headers/defs.h"

extern App app;

void initSDL(void){
    int rendererFlag, windowFlags;

    rendererFlag = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){ // Initialize SDL2's video subsystem
        printf("failed to init video\n");
        exit(1);
    }

    // Create Window Wherever
    app.window = SDL_CreateWindow("Sand Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if(!app.window){
        printf("failed to create window\n");
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlag); // (-1) = use first graphics acceleration device it finds

    if(!app.renderer){
        printf("failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

}
