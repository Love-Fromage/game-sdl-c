#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
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
    int released;
    int bounce;
    float speed;
} ball;

struct palette {
    float x;
    float y;
    float width;
    float height;
    int left;
    int right;
} palette;

struct Brick {
    float x;
    float y;
    float width;
    float height;
    int health;
    int status;
};

struct Brick bricks[10]; // Array to hold the bricks
struct Brick bricks2nd[10]; // Array to hold the bricks

int initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
Mix_Music* music;

void loadSoundEffect(){
    music = Mix_LoadMUS("./sounds/8bit-music-for-game-68698.mp3");
    if(!music){
        printf("failed to load music! SDL_mixer Error : %s\n", Mix_GetError());
    }
}
void playMusic(){
    Mix_PlayMusic(music, -1);
}

void moveBall(float *delta){

    if(ball.y - ball.height < 0){
        ball.up = FALSE;
        ball.bounce = TRUE;
    } 
    if(ball.y + ball.height > WINDOW_HEIGHT){
        ball.up = TRUE;
        ball.bounce = FALSE;
    }
    if(ball.x - ball.width < 0){
        ball.left = !ball.left;
    }
    if(ball.x + ball.width*2 > WINDOW_WIDTH){
        ball.left = !ball.left;
    }
    
    // Check if the ball is within the bounds of the palette
    if(ball.x + ball.width >= palette.x &&
       ball.x - ball.width <= palette.x + palette.width &&
       ball.y + ball.height >= palette.y &&
       ball.y - ball.height <= palette.y + palette.height){

        ball.bounce = FALSE;

    }
    for(int i = 0; i < 10; i++){
        if(bricks[i].status){

            if(ball.x + ball.width >= bricks[i].x &&
                ball.x - ball.width <= bricks[i].x + bricks[i].width &&
                ball.y + ball.height >= bricks[i].y &&
                ball.y - ball.height <= bricks[i].y + bricks[i].height){

                    ball.bounce = TRUE;
                    bricks[i].health = 0;
                    ball.speed +=10; // the ball will accelerate!

            }
        }
        if(bricks[i].health == 0){
            bricks[i].status = FALSE;
        }
        if(bricks2nd[i].status){

            if(ball.x + ball.width >= bricks2nd[i].x &&
                ball.x - ball.width <= bricks2nd[i].x + bricks2nd[i].width &&
                ball.y + ball.height >= bricks2nd[i].y &&
                ball.y - ball.height <= bricks2nd[i].y + bricks2nd[i].height){

                    ball.bounce = TRUE;
                    bricks2nd[i].health = 0;
                    ball.speed +=10; // the ball will accelerate!

            }
        }
        if(bricks2nd[i].health == 0){
            bricks2nd[i].status = FALSE;
        }

    }
    
    
    if(ball.up && !ball.bounce){
        ball.y -= ball.speed * (*delta);
    } else {
        ball.y += ball.speed * (*delta);
    }
    if(ball.left){
        ball.x -= ball.speed * (*delta);
    }
    if(!ball.left){
        ball.x += ball.speed * (*delta);
    }
}

void doKeyDown(SDL_KeyboardEvent *event){
    if(event->repeat == 0){

        if(event->keysym.scancode == SDL_SCANCODE_LEFT){
            palette.left = 1;
        }
        if(event->keysym.scancode == SDL_SCANCODE_RIGHT){
            palette.right = 1;
        }
        
        if(event->keysym.scancode == SDL_SCANCODE_SPACE){
            ball.released = TRUE;
        }
    }
}

void doKeyUp(SDL_KeyboardEvent *event){
    if(event->repeat == 0){
        
        if(event->keysym.scancode == SDL_SCANCODE_LEFT){
            palette.left = 0;
        }
        if(event->keysym.scancode == SDL_SCANCODE_RIGHT){
            palette.right = 0;
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
    ball.left = TRUE;
    ball.width = 15;
    ball.height = 15;
    ball.released = FALSE;
    ball.bounce = FALSE;
    ball.speed = 100;

    palette.width = 100;
    palette.x = WINDOW_WIDTH/2 - palette.width/2;
    palette.y = WINDOW_HEIGHT-100;
    palette.height = 15;

    for(int i = 0; i < 10; i++){
        bricks[i].x = i * (20 + 40)+100; //bricks spaced 20 units apart
        bricks[i].y = 16;
        bricks[i].width = 40; // brick with of 40
        bricks[i].height = 20;
        bricks[i].health = 1; // all bricks have 1 health
        bricks[i].status = TRUE; // brick is initially visible;
        bricks2nd[i].x = i * (20 + 40)+100; //bricks spaced 20 units apart
        bricks2nd[i].y = 55;
        bricks2nd[i].width = 40; // brick with of 40
        bricks2nd[i].height = 20;
        bricks2nd[i].health = 1; // all bricks have 1 health
        bricks2nd[i].status = TRUE; // brick is initially visible;
    }

    
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


    if(palette.left){
        if(palette.x - ball.width < 0){
            palette.x = ball.width;
            palette.left =0;
        }else{
            palette.x -= 350 * delta_time;
        }
    }
    if(palette.right){
        if(palette.x + palette.width > WINDOW_WIDTH){
            palette.x = WINDOW_WIDTH - palette.width;
            palette.right=0;
        }else{
            palette.x += 350 * delta_time;
        }
    }

    // Glue the ball to the palette if the release properties
    if(!ball.released){
        ball.x = palette.x + palette.width/2 - ball.width/2;
        ball.y = palette.y - ball.height;
    } else {
        moveBall(&delta_time);
        ball.up = TRUE;
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0,0,0,250);
    SDL_RenderClear(renderer);

    // Draw blue walls around the edge of the window
    SDL_SetRenderDrawColor(renderer, 0,0,255,255); // Blue color

    // Top wall
    SDL_Rect top_wall = {
        0,
        0,
        WINDOW_WIDTH,
        12,
    };
    SDL_RenderFillRect(renderer, &top_wall);

    // Bootom wall
    SDL_Rect bottom_wall = {
        0,
        WINDOW_HEIGHT-12,
        WINDOW_WIDTH,
        12
    };
    SDL_RenderFillRect(renderer, &bottom_wall);

    // Left wall
    SDL_Rect left_wall = {
        0,
        0,
        12,
        WINDOW_HEIGHT
    };
    SDL_RenderFillRect(renderer, &left_wall);


    // Right wall 
    SDL_Rect right_wall = {
        WINDOW_WIDTH - 12,
        0,
        12,
        WINDOW_HEIGHT
    };
    SDL_RenderFillRect(renderer, &right_wall);


    //Draw a palette
    SDL_Rect palette_rect= {
        palette.x,
        palette.y,
        palette.width,
        palette.height
    };
    // Draw a rectangle
    SDL_Rect ball_rect = {
        ball.x,
        ball.y,
        ball.width,
        ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);
    SDL_RenderFillRect(renderer, &palette_rect);

    SDL_SetRenderDrawColor(renderer, 255,0,0,255); // set the color to red for the bricks

    for(int i = 0; i < 10; i++){
        if(bricks[i].status){
            SDL_Rect brick_rect = {
                bricks[i].x,
                bricks[i].y,
                bricks[i].width,
                bricks[i].height
            };
            SDL_RenderFillRect(renderer, &brick_rect);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0,255,0,255); // set the color to green for the bricks

    for(int i = 0; i < 10; i++){
        if(bricks2nd[i].status){
            SDL_Rect brick_rect = {
                bricks2nd[i].x,
                bricks2nd[i].y,
                bricks2nd[i].width,
                bricks2nd[i].height
            };
            SDL_RenderFillRect(renderer, &brick_rect);
        }
    }


    

    SDL_RenderPresent(renderer);
}

void destroy_window(){
     Mix_FreeMusic(music);
     Mix_CloseAudio();
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