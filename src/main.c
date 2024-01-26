#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

SDL_Window* window = NULL;

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

     window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
     );
    return TRUE;
}

int main(){
    initialize_window();
    return 0;
}