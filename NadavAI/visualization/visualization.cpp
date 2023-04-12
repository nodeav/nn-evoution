#include "visualization.h"
#include <stdexcept>

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

Agent::Agent(const Entity &entity) {
    pos_x = static_cast<int>(entity.location().x() * static_cast<float>(WINDOW_WIDTH));
    pos_y = static_cast<int>(entity.location().y() * static_cast<float>(WINDOW_HEIGHT));
    direction = radiansToDirection(entity.angle());
}

Direction Agent::radiansToDirection(Radian angle) {
    auto degrees = angle.toDegrees();
    if ((degrees >= 0 && degrees <= 45) || (degrees >= 315 && degrees <= 360)) {
        return Direction::down;
    }

    if (degrees < 135) {
        return Direction::right;
    }

    if (degrees < 225) {
        return Direction::up;
    }

    return Direction::left;
}

bool Visualizer::initialized = false;

Visualizer::Visualizer() {
    if (initialized) {
        throw std::runtime_error("Only one instance of Visualizer is allowed");
    }
    initialized = true;

    registerCleanupSignalHandler();

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("SDL2 Sprite Sheets", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, 0);

    image = IMG_Load("./cats-fluffy.png");
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTextureFromSurface(renderer, image);

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
        }

        // move every frame
        if (step >= frames) {
            continue;
        }
        step = frames;


        // rendering
        Sint32 sprite = frames % SPRITE_CYCLE_LEN;
        {
            SDL_RenderClear(renderer);
            std::lock_guard<std::mutex> l(agents_lock);
            for (const auto &agent: agents) {
                SDL_Rect srcrect = {sprite * SPRITE_SIZE, agent.sprite_direction(), SPRITE_SIZE, SPRITE_SIZE};
                SDL_Rect dstrect = {agent.pos_x - AGENT_SIZE / 2, agent.pos_y - AGENT_SIZE / 2, AGENT_SIZE, AGENT_SIZE};

                SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
            }
            SDL_RenderPresent(renderer);
        }

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
    }
}

Visualizer::~Visualizer() {
    stopVizLoop();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Visualizer::updateAgentList(const Board &board) {
    std::lock_guard<std::mutex> l(agents_lock);
    agents.clear();
    for (const auto &entity: board.getEntities()) {
        agents.emplace_back(*entity);
    }
}

void Visualizer::stopVizLoop() {
    quit = true;
}

void Visualizer::registerCleanupSignalHandler() {
    SignalHandler::init([&](int) {
       stopVizLoop();
    });
}
