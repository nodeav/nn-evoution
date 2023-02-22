#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Agent {

// todo: include board and change vis to functions to be consumed from outside

public:        
        int pos_x;
        int pos_y;
        int direction;
        int speed;
        int color;

    void move() {  
        switch (direction) {
            // change to enum (enum classes maybe?)
            case 0:
                pos_x = pos_x + speed;
                break;
            case 1:
                pos_x = pos_x - speed;
                break;
            case 2:
                pos_y = pos_y + speed;
                break;
            case 3:
                pos_y = pos_y - speed;
                break;
        }
    }
};


int main(int argc, char ** argv)
{
    bool quit = false;
    int change_direction_counter = 0;
    int step = 0;
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
 
    SDL_Window * window = SDL_CreateWindow("SDL2 Sprite Sheets", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = IMG_Load("./cats-fluffy.png");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Color color = {0, 0, 0};
    
    SDL_RenderClear(renderer);

    // implement ctor one liner
    Agent agent_1;
    agent_1.pos_x = 500;
    agent_1.pos_y = 500;
    agent_1.direction = 0;
    agent_1.speed = 10;
    while (!quit)
    {
        
        SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
        Uint32 ticks = SDL_GetTicks();
        Uint32 seconds = ticks / 1000;
        Uint32 frames = ticks / 100;

        // change direction
        if (seconds - change_direction_counter == 3) {
            change_direction_counter = seconds;
            agent_1.direction = rand() % 3;
        }

        if (frames > step) {
            step = frames;
            // move
            agent_1.move();
        }

        // rendering
        // todo: remove magic nums
        Uint32 sprite = frames % 3;
        // x, y, width, height
        // todo: remove magic nums
        SDL_Rect srcrect = { sprite * 48, 0, 48, 60 };
        SDL_Rect dstrect = { agent_1.pos_x, agent_1.pos_y, 96, 120 };
        while (SDL_PollEvent(&event) != NULL)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        SDL_RenderPresent(renderer);
    }
 
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
 
    return 0;
}