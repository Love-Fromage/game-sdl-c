#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct ball {
    float x;
    float y;
    float width;
    float height; 
} ball;

int initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

     window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
     );

     if(!window){
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
     }

     renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    return TRUE;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE){
            game_is_running = FALSE;
        }
        break;
    
    default:
        break;
    }

}

void setup(){
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
}

void update(){
    //TODO:
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0,0,0,250);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect1 = {
        ball.x,
        ball.y,
        ball.width,
        ball.height
    };
    SDL_Rect ball_rect2 = {
        ball_rect1.x + ball.width*2,
        ball.y,
        ball.width,
        ball.height
    };
    SDL_Rect ball_rect3 = {
        ball_rect2.x + ball.width*2,
        ball.y,
        ball.width,
        ball.height
    };
    SDL_Rect ball_rect4 = {
        ball_rect3.x + ball.width*2,
        ball.y,
        ball.width,
        ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect1);
    SDL_RenderFillRect(renderer, &ball_rect2);
    SDL_RenderFillRect(renderer, &ball_rect3);
    SDL_RenderFillRect(renderer, &ball_rect4);

    SDL_RenderPresent(renderer);
}

void destroy_window(){
     SDL_DestroyRenderer(renderer); 
     SDL_DestroyWindow(window);
     SDL_Quit();
}

int main(){
    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input();
        update();
        render();
    }
    
    destroy_window();
    
    return 0;
}