
#include <iostream>
#include <string>
#include <random>

#include "board.h"

int main(int argc, char** argv) {


    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " <num_entities>" << std::endl;
        return 1;
    }

    Board board(8, 8);
    uint32_t entities_size = std::stoi(argv[1]);
    float f = 0.3f;

    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, 7);

    for(int i = 0; i < entities_size; i++) {
        int x = uniform_dist(e1);
        int y = uniform_dist(e1);
        board.AddEntity(std::make_shared<Entity>(x, y));
    }

    board.print();
    return 0;

}