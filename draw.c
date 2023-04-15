#include "headers/common.h"
#include "headers/structs.h"
#include "headers/defs.h"

extern App app;

// Functions related to drawing and rendering
void prepareScene(void){
    SDL_SetRenderDrawColor(app.renderer, 128, 128, 128, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void){
    SDL_RenderPresent(app.renderer);
}

void blit(SDL_Texture *texture, float x, float y){
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}
