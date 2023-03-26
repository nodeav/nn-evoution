#include "visualization.h"
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

constexpr int num_entities = 16;

void initBoard(Board &board) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<loc_t> loc_dist(0, 1);
    std::uniform_real_distribution<loc_t> radian_dist(0, 2.0 * M_PI);
    std::uniform_real_distribution<loc_t> speed_dist(0.005, 0.01);


    for (uint32_t i = 0; i < num_entities; i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);

        board.addEntity(std::make_shared<Entity>(x, y, speed, radian, 2 /* radius */));
    }

}

int main(int argc, char **argv) {
    Visualizer vis;
    bool run = true;
    std::thread thread([&vis, &run]() {
        Board board(1, 1);
        initBoard(board);
        while (run) {
            using namespace std::chrono_literals;
            board.moveAll();
            vis.updateAgentList(board);
            std::this_thread::sleep_for(30ms);
        }
    });
    thread.detach();
    vis.startVizLoop();

    run = false;
    thread.join();
}