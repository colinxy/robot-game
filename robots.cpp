#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cassert>
using namespace std;

#include "robots.h"
#include "auxiliary.h"

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c) {
    if (ap == nullptr) {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols()) {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_timesAttacked = 0;
}

int Robot::row() const {
    return m_row;
}

int Robot::col() const {
    return m_col;
}

void Robot::move() {
      // Attempt to move in a random direction; if we can't move, don't move
    int dir = randInt(0, 3);  // dir is now UP, DOWN, LEFT, or RIGHT

    m_arena->determineNewPosition(m_row, m_col, dir);
}

bool Robot::getAttacked(int dir) { // return true if dies
      // If the robot has been attacked once before, return true
      // (since a second attack kills a robot).  Otherwise, if possible, move
      // the robot in one position in direction dir and return false (since
      // it survived the damage).  Otherwise, do not move, but return true
      // (since the momentum from the blow would bump the robot against the
      // wall, dealing it additional fatal damage).

    // robot attacked before
    if (m_timesAttacked == 1) return true;

    bool movable = m_arena->determineNewPosition(m_row, m_col, dir);
    // the robot dies if canot move
    if (!movable) return true;

    // increment the number of times being attacked
    m_timesAttacked ++;

    return false;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c) {
    if (ap == nullptr) {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols()) {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const {
    return m_row;
}

int Player::col() const {
    return m_col;
}

int Player::age() const {
    return m_age;
}

void Player::stand() {
    m_age++;
}

void Player::moveOrAttack(int dir) {
    m_age++;
      // If there is a robot adjacent to the player in the direction
      // dir, attack it.  Otherwise, move the player to that position if
      // possible (i.e., if the move would not be off the edge of the arena).

    // a robot at position row_num, col_num
    int row_num = row();
    int col_num = col();
    m_arena->determineNewPosition(row_num, col_num, dir);

    if (m_arena->nRobotsAt(row_num, col_num) == 0) {
        // no robot at new position
        // move the player
        m_arena->determineNewPosition(m_row, m_col, dir);
    } else {
        // attack robot at new position
        m_arena->attackRobotAt(row_num, col_num, dir);
    }
}

bool Player::isDead() const {
    return m_dead;
}

void Player::setDead() {
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols) {
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena() {
      // Delete the player and all remaining dynamically allocated robots.

    delete m_player;
    for (int i = 0; i < m_nRobots; i++) {
        delete m_robots[i];
    }
}

int Arena::rows() const {
    return m_rows;
}

int Arena::cols() const {
    return m_cols;
}

Player* Arena::player() const {
    return m_player;
}

int Arena::robotCount() const {
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const {
      // Return the number of robots at row r, column c.
    int count = 0;
    for (int i = 0; i < m_nRobots; i++) {
        // Robot* const &robot = m_robots[i];
        if (m_robots[i]->row() == r && m_robots[i]->col() == c) {
            count++;
        }
    }
    return count;
}

bool Arena::determineNewPosition(int& r, int& c, int dir) const {
      // If a move from row r, column c, one step in direction dir
      // would go off the edge of the arena, leave r and c unchanged and
      // return false.  Otherwise, set r or c so that row r, column c, is
      // now the new position resulting from the proposed move, and
      // return true.

    switch (dir) {
      case UP:
        if (r > 1)      r--;
        else            return false;
        break;
      case DOWN:
        if (r < rows()) r++;
        else            return false;
        break;
      case LEFT:
        if (c > 1)      c--;
        else            return false;
        break;
      case RIGHT:
        if (c < cols()) c++;
        else            return false;
        break;
      default:
        return false;
    }
    return true;
}

void Arena::display() const {
      // Position (row,col) in the arena coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

      // Indicate each robot's position
      // If one robot is at some grid point, set the char to 'R'.
      // If it's 2 though 8, set it to '2' through '8'.
      // For 9 or more, set it to '9'.

    for (r = 1; r <= rows(); r++) {
        for (c = 1; c <= cols(); c++) {
            int nRobots = nRobotsAt(r, c);

            if (nRobots == 1) {
                grid[r-1][c-1] = 'R';
            } else if (nRobots >= 2 && nRobots <= 8) {
                grid[r-1][c-1] = '0' + nRobots;
            } else if (nRobots >= 9) {
                grid[r-1][c-1] = '9';
            }
        }
    }

        // Indicate player's position
    if (m_player != nullptr) {
          // Set the char to '@', unless there's also a robot there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++) {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, robot, and player info
    cout << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c) {
      // If MAXROBOTS have already been added, return false.  Otherwise,
      // dynamically allocate a new robot at coordinates (r,c).  Save the
      // pointer to the newly allocated robot and return true.

    if (m_nRobots == MAXROBOTS) {
        return false;
    }
    Robot *robot = new Robot(this, r, c);
    m_robots[m_nRobots++] = robot;
    
    return true;
}

bool Arena::addPlayer(int r, int c) {
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

bool Arena::attackRobotAt(int r, int c, int dir) {
      // Attack one robot at row r, column c if at least one is at
      // that position.  If the robot does not survive the damage, destroy the
      // robot object, removing it from the arena, and return true.  Otherwise,
      // return false (no robot at (r,c), or robot didn't die).

    if (nRobotsAt(r, c) == 0) return false;

    bool robotDead = false;
    int index = 0;
    for (; index < m_nRobots; index++) {
        Robot* &robot = m_robots[index];
        if (robot->row() == r && robot->col() == c) {
            robotDead = robot->getAttacked(dir);
            break;
        }
    }

    if (!robotDead) {
        return false;
    }

    // If the robot does not survive the damage
    delete m_robots[index];
    m_robots[index] = nullptr;
    // decrement the number of robots
    // set the pointer to robot at index to the pointer to the last one
    m_robots[index] = m_robots[--m_nRobots];

    return true;
}

bool Arena::moveRobots() {
    for (int k = 0; k < m_nRobots; k++) {
        // Have the k-th robot in the arena make one move.
        // If that move results in that robot being in the same
        // position as the player, the player dies.

        Robot* &robot = m_robots[k];
        robot->move();
        if (robot->row() == m_player->row() && robot->col() == m_player->col()) {
            m_player->setDead();
        }
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots) {
    if (nRobots < 0) {
        cout << "***** Cannot create Game with negative number of robots!" << endl;
        exit(1);
    }
    if (nRobots > MAXROBOTS) {
        cout << "***** Trying to create Game with " << nRobots
             << " robots; only " << MAXROBOTS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRobots > 0) {
        cout << "***** Cannot create Game with nowhere to place the robots!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0) {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a robot where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addRobot(r, c);
        nRobots--;
    }
}

Game::~Game() {
    delete m_arena;
}

void Game::play() {
    Player* p = m_arena->player();
    if (p == nullptr) {
        m_arena->display();
        return;
    }
    do {
        m_arena->display();
        cout << endl;
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            p->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                p->moveOrAttack(decodeDirection(action[0]));
                break;
            }
        }
        m_arena->moveRobots();
    } while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0);
    m_arena->display();
}
