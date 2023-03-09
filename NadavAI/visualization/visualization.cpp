#include "visualization.h"
#include <stdexcept>

void Agent::move() {
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

int Agent::sprite_direction() const {
    switch (direction) {
        case Direction::right:
            return 96;
        case Direction::left:
            return 48;
        case Direction::up:
            return 0;
        case Direction::down:
            return 144;
    }
}

bool Visualizer::initialized = false;

Visualizer::Visualizer() {
    if (initialized) {
        throw std::runtime_error("Only one instance of Visualizer is allowed");
    }
    initialized = true;
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("SDL2 Sprite Sheets", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000,
                              1000, 0);

    image = IMG_Load("./cats-fluffy.png");
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTextureFromSurface(renderer, image);

    agent_1.pos_x = 500;
    agent_1.pos_y = 500;
    agent_1.direction = Agent::Direction::right;
    agent_1.speed = 10;

    SDL_RenderClear(renderer);
}

void Visualizer::startVizLoop() {
    SDL_Event event;

    while (!quit) {

        SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
        Uint32 ticks = SDL_GetTicks();
        Uint32 seconds = ticks / 1000;
        Uint32 frames = ticks / 100;

        // change direction every 3 seconds
        if (seconds - change_direction_counter == 3) {
            change_direction_counter = seconds;
            agent_1.direction = static_cast<Agent::Direction>(rand() % 3);
        }

        // move every frame
        if (frames > step) {
            step = frames;
            agent_1.move();
        }

        // rendering
        Sint32 sprite = frames % SPRITE_CYCLE_LEN;
        // x, y, width, height

        SDL_Rect srcrect = {sprite * SPRITE_SIZE, agent_1.sprite_direction(), SPRITE_SIZE, SPRITE_SIZE};
        SDL_Rect dstrect = {agent_1.pos_x, agent_1.pos_y, AGENT_SIZE, AGENT_SIZE};
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        SDL_RenderPresent(renderer);
    }
}

Visualizer::~Visualizer() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}