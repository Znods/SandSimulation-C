#include "headers/common.h"
#include "headers/defs.h"
#include "headers/structs.h"
#include "headers/util.h"
#include "headers/input.h"
#include "headers/init.h"
#include "headers/draw.h"

/*
Left Cntrl = Add Sand
Right Cntrl = Remove Sand
*/

//SDL_Texture *sand_p;

int sand_id = 1;
bool running = 1;
struct Pixels *head = NULL;
struct Texture textureHead, *textureTail;
int keyboard[MAX_KEYBOARD_KEYS];
App app;

static int pixelmap[SCREEN_WIDTH][SCREEN_HEIGHT];

void displayList(void){
    struct Pixels *current = head;

    if(current != NULL){
        while(current != NULL){
            printf("s:%d --> ", current->id);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void removePixel(void){
    if(head != NULL){
        struct Pixels *temp = head->next;
        pixelmap[head->x][head->y] = 0;
        free(head);
        head = temp;
    }
}

void addPixel(void){
    struct Pixels *new = (struct Pixels *)malloc(sizeof(struct Pixels));
    if(new == NULL){
        fprintf(stderr, "can't allocate memory!\n");
        exit(1);
    }
    new->x = 800;
    new->y = 100;
    new->dx = 0;
    new->dy = 0;
    new->id = sand_id++;
    new->status = 1;
    new->next = head;

    head = new;
}

void drawFloor(void){
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < 8; j++) {
            SDL_RenderDrawPoint(app.renderer, 0 + i, (FLOOR + 4) + j);
        }
    }
}

void drawPixels(void){

    struct Pixels *current = head;

    if(current != NULL){
        while(current != NULL){
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    SDL_RenderDrawPoint(app.renderer, current->x + i, current->y + j);
                }
            }
            current = current->next;
        }
    }
}

void doPixels(void){
    struct Pixels *sand = head;

    if(sand != NULL){
        while(sand != NULL){
            sand->x += sand->dx;
            sand->y += sand->dy;
            sand = sand->next;
        }
    }

}

void doPixelMap(void){
    struct Pixels *sand = head;

    if(sand != NULL){
        while(sand != NULL){
            if(pixelmap[sand->x][sand->y + 4] == 0 && sand->y < FLOOR){ // if down is empty, move down
                pixelmap[sand->x][sand->y] = 0;
                sand->y += 4;
                pixelmap[sand->x][sand->y] = 1;
                printf("sand[%d] --> down\n", sand->id);
            } else if(pixelmap[sand->x - 4][sand->y + 4] == 0 && sand->y < FLOOR){ // if down and left are empty, move down and left
                pixelmap[sand->x][sand->y] = 0;
                sand->y += 4;
                sand->x -= 4;
                printf("sand[%d] --> down & left\n", sand->id);
                pixelmap[sand->x][sand->y] = 1;
            } else if(pixelmap[sand->x + 4][sand->y + 4] == 0 && sand->y < FLOOR){ // if down and right are empty, move down and right
                pixelmap[sand->x][sand->y] = 0;
                sand->y += 4;
                sand->x += 4;
                pixelmap[sand->x][sand->y] = 1;
                printf("sand[%d] --> down & right\n", sand->id);
            } else {
                // stay
            }
            sand = sand->next;
        }
    }
}

void collision(void){
    struct Pixels *sand = head;
    struct Pixels *prev = NULL;

    if(sand != NULL){
        while(sand != NULL){
            if(prev != NULL){
                //printf("prev = %d\n", prev->y);
                if((sand->y + sand->x) == (prev->y + prev->x)){
                    pixelmap[sand->x][sand->y] = 0;
                    sand->y -= 4;
                    pixelmap[sand->x][sand->y] = 1;
                }
            }
            prev = sand;
            sand = sand->next;
        }
    }
}

void clipPixels(void){
    struct Pixels *sand = head;

    if(sand != NULL){
        while(sand != NULL){
            if(sand->y > FLOOR){
                sand->dy = 0;
                //puts("clipping sand...");
                pixelmap[sand->x][sand->y] = 1;
                collision();
            }
            sand = sand->next;
        }
    }
}

void initPMap(void){
    for(int i = 0; i < SCREEN_WIDTH; i++){
        for(int j = 0; j < SCREEN_HEIGHT; j++){
            pixelmap[i][j] = 0;
        }
    }
}

void free_pixels(void){
    struct Pixels *temp;
    while(head != NULL) {
        temp = head->next;
        free(head);
        head = temp;
    }
    puts("Pixels Cleared!");
}

static void logic(void){
    //doPixels();
    clipPixels();
    doPixelMap();
}

static void draw(void){
    drawPixels();
    drawFloor();
}

int main(void){
    long then;
    float remainder;

    memset(&app, 0, sizeof(App));
    memset(keyboard, 0, sizeof(keyboard));

    app.delegate.logic = logic;
    app.delegate.draw = draw;

    textureTail = &textureHead;
    //sand_p = loadTexture("cells.jpg");

    initSDL();
    initPMap();

    then = SDL_GetTicks();
    remainder = 0;

    while(running){
        prepareScene(); // sets color for background and clear background

        doInput();

        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255); // sets color for pixels

        registerInput();

        app.delegate.logic();
        app.delegate.draw();
        
        presentScene();

        capFrameRate(&then, &remainder); // caps to around 60 fps
    }

    when_exit();

    return 0;
}
