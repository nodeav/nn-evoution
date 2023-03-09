#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int sprite_cycle_len = 3;
const int sprite_size = 48;
const int agent_size = 96;
class Agent {

public:        
        enum class Direction {right, left, up, down};
        int pos_x;
        int pos_y;
        Direction direction;
        int speed;
        int color;

    void move() {  
        switch (direction) {
            case Direction::right:
                pos_x = pos_x + speed;
                break;
            case Direction::left:
                pos_x = pos_x - speed;
                break;
            case Direction::up:
                pos_y = pos_y + speed;
                break;
            case Direction::down:
                pos_y = pos_y - speed;
                break;
        }
    }

    int sprite_direction() {
        switch (direction) {
            case Direction::right:
                return 96;
                break;
            case Direction::left:
                return 48;
                break;
            case Direction::up:
                return 0;
                break;
            case Direction::down:
                return 144;
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
    agent_1.direction = Agent::Direction::right;
    agent_1.speed = 10;
    while (!quit)
    {
        
        SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
        Uint32 ticks = SDL_GetTicks();
        Uint32 seconds = ticks / 1000;
        Uint32 frames = ticks / 100;

        // change direction every 3 seconds
        if (seconds - change_direction_counter == 3) {
            change_direction_counter = seconds;
            agent_1.direction = static_cast<Agent::Direction>(rand() % 3);
            // std::cout << agent_1.direction << std::endl;
        }

        // move every frame
        if (frames > step) {
            step = frames;
            agent_1.move();
        }

        // rendering
        // todo: remove magic nums
        Sint32 sprite = frames % sprite_cycle_len;
        // x, y, width, height
        // todo: remove magic nums
        SDL_Rect srcrect = { sprite * sprite_size, agent_1.sprite_direction(), sprite_size, sprite_size };
        SDL_Rect dstrect = { agent_1.pos_x, agent_1.pos_y, agent_size, agent_size };
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