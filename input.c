#include "headers/common.h"
#include "headers/defs.h"

extern bool running;
extern int keyboard[MAX_KEYBOARD_KEYS];

void addPixel(void);
void removePixel(void);
void displayList(void);
void free_pixels(void);

void registerInput(void){
    if(keyboard[SDL_SCANCODE_LCTRL]){
        printf("Adding pixel to screen!\n");
        addPixel();
    } 
    if(keyboard[SDL_SCANCODE_RCTRL]){
        printf("Displaying list...\n");
        removePixel();
        displayList();
    }
}

// Handles input such as reading the keyboard
static void doKeyDown(SDL_KeyboardEvent *event){
    if(event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS){
        keyboard[event->keysym.scancode] = 1;
    }
}

static void doKeyUp(SDL_KeyboardEvent *event){
    if(event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS){
        keyboard[event->keysym.scancode] = 0;
    }
}

void doInput(void){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
            default:
                break;
        }
    }
}
