#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

constexpr int SPRITE_CYCLE_LEN = 3;
constexpr int SPRITE_SIZE = 48;
constexpr int AGENT_SIZE = 96;

class Agent {

public:
    enum class Direction {
        right, left, up, down
    };
    int pos_x;
    int pos_y;
    Direction direction;
    int speed;
    int color;

    void move();

    int sprite_direction() const;
};

class Visualizer {
    bool quit = false;
    int step = 0;
    int change_direction_counter = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *image;
    SDL_Texture *texture;
    Agent agent_1;


public:
    Visualizer();

    ~Visualizer();

    void startVizLoop();
};