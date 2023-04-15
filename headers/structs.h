#include "common.h"
#include "defs.h"

typedef struct {
    void (*logic)(void);
    void (*draw)(void);
} Delegate;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Delegate delegate;
} App;

typedef struct Texture{
    char name[MAX_NAME_LENGTH];
    SDL_Texture *texture;
    struct Texture *next;
} Texture;

typedef struct Pixels{
    int x;
    int y;
    int dx;
    int dy;
    int id;
    bool status;
    struct Pixels *next;
}pixels_t;

