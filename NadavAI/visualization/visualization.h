#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <thread>

#include <csignal>

#include "entities.h"
#include "board.h"

constexpr int SPRITE_CYCLE_LEN = 3;
constexpr int SPRITE_SIZE = 48;
constexpr int AGENT_SIZE = 96;

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 1024;

enum class Direction {
    right, left, up, down
};

class Agent {

public:
    explicit Agent(const Entity &entity);

    int pos_x;
    int pos_y;
    Direction direction;

    int sprite_direction() const;

    static Direction radiansToDirection(Radian angle);
};

// TODO: this is a realtime-only visualization
class Visualizer {
    bool quit = false;
    int step = 0;
    int change_direction_counter = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *image;
    SDL_Texture *texture;
    std::vector<Agent> agents;
    static bool initialized;
    std::mutex agents_lock;

    void registerCleanupSignalHandler();
public:
    Visualizer();

    ~Visualizer();

    void updateAgentList(const Board &board);

    void startVizLoop();
    void stopVizLoop();
};

// TODO: move to util
// TODO: probably unnecessary for now,
// but will be useful for e.g saving on exit or something
class SignalHandler {
    using handler_type = std::function<void(int)>;
public:
    static inline handler_type handler;
    static void signalHandler(int signal) {
        handler(signal);
    }
    static void init(handler_type new_handler) {
        handler = std::move(new_handler);
        std::signal(SIGINT, signalHandler);
        std::signal(SIGABRT, signalHandler);
    }
};