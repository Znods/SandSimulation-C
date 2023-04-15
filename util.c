#include "headers/common.h"
#include "headers/structs.h"
#include "headers/defs.h"

extern App app;
extern struct Texture textureHead, *textureTail;

void free_pixels(void);

void when_exit(void){
    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    //SDL_DestroyTexture(sand_p);
    free_pixels();
    SDL_Quit();
}

void capFrameRate(long *then, float *remainder){
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if(wait < 1){
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;
    
    *then = SDL_GetTicks();

}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture){
    Texture *texture;

    texture = malloc(sizeof(struct Texture));
    memset(texture, 0 , sizeof(struct Texture));
    textureTail->next = texture;
    textureTail = texture;

    STRNCPY(texture->name, name, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;
}

static SDL_Texture *getTexture(char *name){
    Texture *t;

    for(t = textureHead.next; t != NULL; t = t->next){
        if(strcmp(t->name, name) == 0){
            return t->texture;
        }
    }
    return NULL;
}

SDL_Texture *loadTexture(char *filename){
    SDL_Texture *texture;

    texture = getTexture(filename);

    if(texture == NULL){

        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading... %s", filename);

        texture = IMG_LoadTexture(app.renderer, filename);

        addTextureToCache(filename, texture);
    }

    return texture;
}
