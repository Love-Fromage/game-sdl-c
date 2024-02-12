#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int last_frame_time = 0;

struct ball {
    float x;
    float y;
    float width;
    float height; 
    int up;
    int down;
    int left;
    int right;
} ball;

int initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

     window = SDL_CreateWindow(
        "TEST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_MINIMIZED
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
void doKeyDown(SDL_KeyboardEvent *event){
    if(event->repeat == 0){
        if(event->keysym.scancode == SDL_SCANCODE_UP){
            ball.up = 1;
        }
        if(event->keysym.scancode == SDL_SCANCODE_DOWN){
            ball.down = 1;
        }
        if(event->keysym.scancode == SDL_SCANCODE_LEFT){
            ball.left = 1;
        }
        if(event->keysym.scancode == SDL_SCANCODE_RIGHT){
            ball.right = 1;
        }
    }
}

void doKeyUp(SDL_KeyboardEvent *event){
    if(event->repeat == 0){
        if(event->keysym.scancode == SDL_SCANCODE_UP){
            ball.up = 0;
        }
        if(event->keysym.scancode == SDL_SCANCODE_DOWN){
            ball.down = 0;
        }
        if(event->keysym.scancode == SDL_SCANCODE_LEFT){
            ball.left = 0;
        }
        if(event->keysym.scancode == SDL_SCANCODE_RIGHT){
            ball.right = 0;
        }
    }
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
        }else {
            doKeyDown(&event.key);
            break;
        }
        break;

    case SDL_KEYUP:
        doKeyUp(&event.key);
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
    // Waste some time /  sleep  until we  reach the frame target time
    // while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // Calculate how much we have to wait until we reach the target frame time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() -last_frame_time);

    // Only delay if we are too fast too update this frame
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }


    // Get a delta time factor converted to seconds to be used to update my objects
    float delta_time = (SDL_GetTicks()-last_frame_time)/1000.0f;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    // Move ball as a function of delta time
    // ball.x += 20 * delta_time;
    // ball.y += 30 * delta_time;

    if(ball.up){
        if(ball.y - ball.height < 0){
            ball.y = ball.y;
            ball.up=0;
        }else{
            ball.y -= 100 * delta_time;
        }
    }
    if(ball.down){
        if(ball.y + ball.height > WINDOW_HEIGHT){
            ball.y = WINDOW_HEIGHT - ball.height;
            ball.down=0;
        }else{
            ball.y += 100 * delta_time;
        }
    }
    if(ball.left){
        if(ball.x - ball.width < 0){
            ball.x = ball.width;
            ball.left =0;
        }else{
            ball.x -= 100 * delta_time;
        }
    }
    if(ball.right){
        if(ball.x + ball.width > WINDOW_WIDTH){
            ball.x = WINDOW_WIDTH - ball.width;
            ball.right=0;
        }else{
            ball.x += 100 * delta_time;
        }
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0,0,0,250);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect = {
        ball.x,
        ball.y,
        ball.width,
        ball.height
    };
    // SDL_Rect ball_rect2 = {
    //     ball_rect1.x + ball.width*2,
    //     ball.y,
    //     ball.width,
    //     ball.height
    // };
    // SDL_Rect ball_rect3 = {
    //     ball_rect2.x + ball.width*2,
    //     ball.y,
    //     ball.width,
    //     ball.height
    // };
    // SDL_Rect ball_rect4 = {
    //     ball_rect3.x + ball.width*2,
    //     ball.y,
    //     ball.width,
    //     ball.height
    // };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);
    // SDL_RenderFillRect(renderer, &ball_rect2);
    // SDL_RenderFillRect(renderer, &ball_rect3);
    // SDL_RenderFillRect(renderer, &ball_rect4);

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