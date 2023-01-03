
#include <iostream>
#include <string>
#include <random>

#include "board.h"

/* Rules :
- entity can eat to its angle, in an ROI.
- as an MVP, entities should be approximately round.
- an entity can be eaten if its center is in a predetor's ROI.
- grid is continuous and has no borders

*/

using std::cout;
using std::endl;

int main(int argc, char** argv) {


    if (argc < 3) {
        cout << "USAGE: " << argv[0] << " <num_entities> <num_frames>" << endl;
        return 1;
    }

    Board board(8, 8);
    uint32_t entities_size = std::stoi(argv[1]);
    uint64_t frames_size = std::stoi(argv[2]);

    std::random_device r;

    std::default_random_engine e1(r());
    std::uniform_real_distribution<loc_t> loc_dist(0, 8);
    std::uniform_real_distribution<loc_t> radian_dist(0, 2.0 * M_PI);
    std::uniform_real_distribution<loc_t> speed_dist(0, 1);


    for(int i = 0; i < entities_size; i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);

        board.AddEntity(std::make_shared<Entity>(x, y, speed, radian, 2));
    }

    cout << "Start:" << endl;
    board.print();

    for(int i = 0; i < frames_size; ++i) {
        board.moveAllEntities();

        cout << "After Move:" << endl;
        board.print();
    }

    return 0;

}