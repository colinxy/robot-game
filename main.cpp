#include <iostream>
#include <cstdlib>
using namespace std;

#include "robots.h"

int main(int argc, char* argv[]) {
    bool fromCmd = false;
    int rows, cols, nRobots;
    if (argc == 4) {
        fromCmd = true;
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
        nRobots = atoi(argv[3]);
        // cout << rows << ' ' << cols << ' ' << nRobots << endl;
        if (rows < 1 || cols < 1 || nRobots < 1) {
            fromCmd = false;
        }
    }

    Game g(fromCmd ? rows : 7,
           fromCmd ? cols : 8,
           fromCmd ? nRobots : 25);

      // Play the game
    g.play();

    return 0;
}
