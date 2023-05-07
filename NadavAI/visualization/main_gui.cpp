#include <QApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include "visualization.h"
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

constexpr int num_entities = 16;

// to do make size of entity (sdl), amount of entities and board size configurable from qt

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

int main(int argc, char *argv[]) {

    int frame_num = 0;

    std::thread qt_thread([&argc, &argv, &frame_num]() {
        QApplication app(argc, argv);
        QWidget window;

        window.resize(1000, 1000);
        window.setWindowTitle("Simple example");
        window.show();
        QLabel hello( "Hello world!", &window );
        hello.resize( 100, 30 );
        hello.move( 10, 10 );

        std::thread setText_thread([&argc, &argv, &frame_num, &hello]() {
            int last_update = 0;
            while (true) {
                using namespace std::chrono_literals;
                int frames_passed = frame_num - last_update;
                last_update = frame_num;
                hello.setText(("fps: " + std::to_string(frames_passed)).c_str());
                std::this_thread::sleep_for(1s);
            }
        });

        setText_thread.detach();

        hello.show();
        app.exec();

        if (setText_thread.joinable()) {
            setText_thread.join();
        } 
    });

    qt_thread.detach();


    Visualizer vis;
    bool run = true;
    std::thread thread([&vis, &run, &frame_num]() {
        Board board(1, 1);
        initBoard(board);
        while (run)
        {
            using namespace std::chrono_literals;
            board.moveAll();
            vis.updateAgentList(board);
            frame_num++;
        }
    });
    thread.detach();
    vis.startVizLoop();

    run = false;

    if (thread.joinable()) {
        thread.join();
    }
    if (qt_thread.joinable()) {
        qt_thread.join();
    }
}
