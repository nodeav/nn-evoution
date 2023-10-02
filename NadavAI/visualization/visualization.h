#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <thread>

#include <csignal>

#include "entities.h"
#include "board.h"

constexpr int SPRITE_CYCLE_LEN = 3;
constexpr int SPRITE_SIZE = 48;
constexpr int AGENT_SIZE = 36;

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 1024;

enum class Direction {
    right, left, up, down
};

enum class Type {
    cat, mouse
};

class Agent {

public:
    explicit Agent(const Entity &entity);

    int pos_x;
    int pos_y;
    Direction direction;

    int sprite_direction() const;

    static Direction radiansToDirection(Radian angle);
    Type type;
};

// TODO: this is a realtime-only visualization
class Visualizer {
    bool quit = false;
    int step = 0;
    int change_direction_counter = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *cats_image;
    SDL_Surface *mice_image;
    SDL_Texture *cats_texture;
    SDL_Texture *mice_texture;
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