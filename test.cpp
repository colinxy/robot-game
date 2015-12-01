#include <iostream>
#include <cassert>
using namespace std;

#include "robots.h"
#define ROBOT_ATTACKED m_timesAttacked
#define OP +

inline int abs(int x) {
    return (x > 0) ? x : -x;
}


/*******************
**** Robot test ****
********************/

void robot_test() {
    Arena arena = Arena(3, 5);
    Robot robot = Robot(&arena, 1, 1);

    // void Robot::move()
    int prev_row = robot.row();
    int prev_col = robot.col();

    for (int i = 0; i < 10; ++i) {
        robot.move();

        if (!(abs(robot.row() - prev_row) + abs(robot.col() - prev_col) <= 1)) {
            cout << "FAILURE" << endl
                 << "void Robot::move()" << endl;
            assert(false);
        }

        prev_row = robot.row();
        prev_col = robot.col();
    }

    // bool getAttacked(int dir)
    // starting position (2, 2)
    robot = Robot(&arena, 2, 2);
    int times_attacked = robot.ROBOT_ATTACKED;
    bool returnval = robot.getAttacked(UP);
    // the robot gets attacked the first time
    assert(robot.ROBOT_ATTACKED == 1);
    // robot did not die
    assert(returnval == false);
    // if possible, move the robot in one position in direction
    assert(robot.row() == 1 && robot.col() == 2);
    assert(robot.ROBOT_ATTACKED == (times_attacked OP 1));
    // robot gets attacked again
    returnval = robot.getAttacked(LEFT);
    assert(returnval == true);

    // starting position (3, 2)
    robot = Robot(&arena, 3, 2);
    times_attacked = robot.ROBOT_ATTACKED;
    returnval = robot.getAttacked(DOWN);
    assert(returnval == true);
    assert(robot.row() == 3 && robot.col() == 2);
}


/*******************
*** Player test ****
********************/

void player_test() {
    
    
    
    Arena *arena = new Arena(5, 5);
    arena->addRobot(4, 3);
    arena->addRobot(5, 4);
    arena->addRobot(4, 3);
    arena->addPlayer(4, 4);
    arena->display();

    arena->m_player->moveOrAttack(LEFT);
    arena->display();

    arena->m_player->moveOrAttack(LEFT);
    arena->display();

    // kills a robot
    arena->m_player->moveOrAttack(DOWN);
    arena->display();

    arena->m_player->moveOrAttack(LEFT);
    arena->display();

    arena->m_player->moveOrAttack(LEFT);
    arena->display();

    arena->m_player->moveOrAttack(LEFT);
    arena->display();

    delete arena;
    
    arena = new Arena(1, 3);
    arena->addRobot(1, 1);
    arena->addPlayer(1, 2);
    arena->display();

    arena->determineNewPosition(
        arena->m_robots[0]->m_row, 
        arena->m_robots[0]->m_col, 
        RIGHT
    );
    arena->m_player->setDead();
    arena->display();
    
    delete arena;
}


/*******************
**** Arena test ****
********************/

void arena_test() {
    Arena *arena = new Arena(5, 5);
    arena->addPlayer(4, 4);
    for (int i = 0; i < 10; ++i) {
        arena->addRobot(3, 4);
    }
    arena->addRobot(4, 3);
    arena->addRobot(1, 2);
    assert(arena->robotCount() == 12);
    assert(arena->nRobotsAt(3, 4) == 10);
    arena->display();

    arena->determineNewPosition(
        arena->m_robots[arena->m_nRobots-1]->m_row,
        arena->m_robots[arena->m_nRobots-1]->m_col,
        UP
    );
    arena->display();
    
    arena->addRobot(4, 4);
    assert(arena->robotCount() == 13);
    arena->m_player->setDead();
    arena->display();

    delete arena;

    // bool Arena::attackRobotAt(int r, int c, int dir)
    arena = new Arena(3, 4);
    arena->addRobot(1, 3);
    arena->addRobot(2, 2);
    arena->addRobot(3, 2);
    arena->addPlayer(2, 3);
    arena->display();

    assert(arena->attackRobotAt(2, 2, LEFT) == false);
    assert(arena->m_nRobots == 3);
    arena->display();

    assert(arena->attackRobotAt(1, 3, UP) == true);
    arena->display();
    assert(arena->m_nRobots == 2);

    assert(arena->attackRobotAt(2, 4, RIGHT) == false);
    arena->display();
    assert(arena->m_nRobots == 2);

    delete arena;

    // bool Arena::moveRobots()
    arena = new Arena(1, 1);
    arena->addRobot(1, 1);
    arena->addPlayer(1, 1);
    arena->display();

    assert(arena->moveRobots() == false);
    assert(arena->nRobotsAt(1, 1) == 1);
    assert(arena->m_player->isDead() == true);
    arena->display();

    delete arena;
}


int main()
{
    robot_test();
    player_test();
    arena_test();
    cout << "all test passed" << endl;
    return 0;
}
