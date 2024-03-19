
#include <iostream>
#include <string>
#include <random>
#include <stdexcept>

#include "board.h"
// JIRA
/* Rules :
- entity can eat to its angle, in an ROI.
- as an MVP, entities should be approximately round.
- an entity can be eaten if its center is in a predetor's ROI.
- grid is continuous and has no borders


Up Next:
1. implement energy                              V
    a. for toref:
        i.  if depleted -> die                   V
        ii. to gain energy -> eat                V

    b. for tarif:
        i.  if depleted -> don't move            V
        ii. to gain energy -> wait               V

2. implement birth mechanism                     V
    a. copy                                      V
        a. for tarif - if age > 90               V
        b. for toref - if ate 3 entities         V
    b. mutate

3. adjust smart evolution:
    a. copying the net to child                  V
    b. add a mutation                            V

4. implement cooldown for toref after eating     V

5. improve birth mechanism and decay <- The fu**

5.5. Telemetry / Statistics:
 a. Calculations
 b. UI

5.6. "Start Simulation" window
 a. configuration
 b. UI controllers

5.7. UI: 2D visualization: only 'up' sprite but rotated V

6. optimization for getEntitiesInFov

7. reinforcement learning

8. think about how to make them smarter
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


    for(uint32_t i = 0; i < entities_size / 2; i++) {
        loc_t x = loc_dist(e1);
        loc_t y = loc_dist(e1);
        speed_t speed = speed_dist(e1);
        Radian radian = radian_dist(e1);

        board.addToref(std::make_shared<Toref>(x, y, speed, radian, 2 /* radius */));
        board.addTarif(std::make_shared<Tarif>(x, y, speed, radian, 2 /* radius */));
    }

//    cout << "Start:" << endl;
//    board.print();

    for(uint64_t i = 0; i < frames_size; ++i) {
        board.moveAll();
    }

    return 0;

}