#include "visualization.h"
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

constexpr int num_entities = 1000;

void initBoard(Board &board) {
#ifdef CAT_GPT_REPRODUCIBLE
    auto rd = []() { return 42; };
#else
    static thread_local std::random_device rd{};
#endif
    std::default_random_engine e1(rd());
    std::uniform_real_distribution<loc_t> loc_dist(0, 1);
    std::uniform_real_distribution<loc_t> radian_dist(0, 2.0 * M_PI);
    std::uniform_real_distribution<loc_t> speed_dist(0.003, 0.03);

    auto percent_tarif = 0.5;

    for (uint32_t i = 0; i < num_entities * percent_tarif; i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);
        board.addEntity(std::make_shared<Tarif>(x, y, speed, radian, 2 /* radius */));
    }

    for (auto i = 0; i < num_entities * (1 - percent_tarif); i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);
        board.addEntity(std::make_shared<Toref>(x, y, speed, radian, 2 /* radius */));
    }

}

int main(int argc, char **argv) {
    srand((unsigned int) time(nullptr));
    timer main("main");
    int max_iters = -1;
    if (argc > 1) {
        max_iters = static_cast<int>(std::strtol(argv[1], nullptr, 10));
        std::cout << "will run " << max_iters << " iterations" << std::endl;
    }

    Visualizer vis;
    bool run = true;
    std::thread thread([&vis, &run, &max_iters]() {
        Board board(1, 1);
        initBoard(board);
        while (run && max_iters--) {
//            using namespace std::chrono_literals;
//            std::chrono::steady_clock::time_point begin;
            board.moveAll();
            vis.updateAgentList(board);
            if (board.getEntities().empty()) {
                std::cout << "simulation done with gen " << board.gen << std::endl;
                break;
            }
//            std::chrono::steady_clock::time_point end;
//            auto elapsed = end - begin;
//            auto sleep = 15ms - elapsed;
//            std::this_thread::sleep_for(sleep);
        }
        vis.stopVizLoop();
    });
    thread.detach();
    vis.startVizLoop();

    run = false;

    if (thread.joinable()) {
        thread.join();
    }
}