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
    type = (entity.getType() == EntityType::TOREF ? Type::cat : Type::mouse);
}


Direction Agent::radiansToDirection(Radian angle) {
    if (angle.between(Radian::fromDegrees(225.0), Radian::fromDegrees(315.0))) {
        return Direction::down;
    }
    if (angle.between(Radian::fromDegrees(315.0), Radian::fromDegrees(45.0))) {
        return Direction::right;
    }
    if (angle.between(Radian::fromDegrees(45.0), Radian::fromDegrees(135.0))) {
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

    renderer = SDL_CreateRenderer(window, -1, 0);

    cats_image = IMG_Load("./cats-fluffy.png");
    cats_texture = SDL_CreateTextureFromSurface(renderer, cats_image);

    mice_image = IMG_Load("./mice-fluffy.png");
    mice_texture = SDL_CreateTextureFromSurface(renderer, mice_image);

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

                if (agent.type == Type::cat) {
                    SDL_RenderCopy(renderer, cats_texture, &srcrect, &dstrect);
                } else {
                    SDL_RenderCopy(renderer, mice_texture, &srcrect, &dstrect);
                }
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
    SDL_DestroyTexture(cats_texture);
    SDL_DestroyTexture(mice_texture);
    SDL_FreeSurface(cats_image);
    SDL_FreeSurface(mice_image);
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
