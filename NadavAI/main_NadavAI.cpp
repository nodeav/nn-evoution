
#include <iostream>
#include <string>
#include <random>
#include <stdexcept>

#include "board.h"

/* Rules :
- entity can eat to its angle, in an ROI.
- as an MVP, entities should be approximately round.
- an entity can be eaten if its center is in a predetor's ROI.
- grid is continuous and has no borders

*/

using std::cout;
using std::endl;

int printUsageAndReturnError(char *program_name) {
    cout << "USAGE: " << program_name << " <num_entities> <num_frames>" << endl;
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        return printUsageAndReturnError(argv[0]);
    }
    uint32_t entities_size;
    uint64_t frames_size;
    try {
        entities_size = std::stoi(argv[1]);
        frames_size = std::stoi(argv[2]);
    } catch (const  std::exception& ex) {
        return printUsageAndReturnError(argv[0]);
    }


    Board board(8, 8);

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<loc_t> loc_dist(0, 8);
    std::uniform_real_distribution<loc_t> radian_dist(0, 2.0 * M_PI);
    std::uniform_real_distribution<loc_t> speed_dist(0, 1);


    for(uint32_t i = 0; i < entities_size; i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);

        board.addEntity(std::make_shared<Entity>(x, y, speed, radian, 2 /* radius */));
    }

//    cout << "Start:" << endl;
//    board.print();

    for(uint64_t i = 0; i < frames_size; ++i) {
        board.moveAll();
    }

    return 0;

}