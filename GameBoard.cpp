/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Original Author      : Keunhong Park
// Original Source      : https://github.com/void42/Reversi

#include <iostream>
#include <cstdio>
#include <cassert>

#include "GameBoard.hpp"

/**
 * Construct a board with four intial pieces
 */
GameBoard::GameBoard(int _size) : size(_size)
{
    assert(size > 2);

    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    status = 1;

    // allocate memory for gameboard matrix
    gameboard = new int*[size];
    for (int i = 0; i <= size; i++){
        gameboard[i] = new int[size];
    }
        
    // initialize board by setting all slots to 0
    for (int row = 0; row <= size; row++)
        for (int col = 0; col <= size; col++)
            set_slot(row, col, 0);

    // set initial pieces
    set_slot(size/2 - 1, size/2 - 1, 1);
    set_slot(size/2, size/2, 1);
    set_slot(size/2 - 1, size/2, 2);
    set_slot(size/2, size/2 - 1, 2);
}

GameBoard::GameBoard(GameBoard* g)
{
   int size = g->get_size();
   this->size = size;
   //std::cout << "gamesize: " << size << std::endl;

    gameboard = new int*[size];
    for (int i = 0; i <= size; i++){
        gameboard[i] = new int[size];
    }
   for (int i = 0; i < size; i++) {
       for (int j = 0; j < size; j++) {
           // TODO: optimize this!!!
           gameboard[i][j] = g->get_slot_status(i, j);
       }
   }
}

/**
 * Return the status of a slot
 */
int GameBoard::get_slot_status(int x, int y) const
{
    assert(x >= 0 && y >= 0);
    //std::cout << "X, Y: " << x << ", " << y<< std::endl;
    assert(x <= size && y <= size);
    return gameboard[x][y];
}

/**
 * Getter for size of board
 */
int GameBoard::get_size() const
{
    return size;
}

/**
 * indicates whether a move is valid
 */
bool GameBoard::is_valid_move(int x, int y, int player) const
{
    assert(x >= 0 && y >= 0);
    assert(x <= size && y <= size);

    // check if slot is already taken
    if (get_slot_status(x,y) != 0)
        return false;

    // must flip more than 0 stones
    return (flip_count(x, y, player) > 0);
}

/**
 * assign a slot to a player
 */
void GameBoard::set_slot(int x, int y, int player)
{
    assert(x >= 0 && y >= 0);
    assert(x <= size && y <= size);
    gameboard[x][y] = player;
    change_score(1, player);
}

/**
 * Count number of flips a move will make
 */
int GameBoard::flip_count(int x, int y, int player) const
{
    assert(x >= 0 && y >= 0);
    assert(x <= size && y <= size);

    // dir_x and dir_y are each -1, 0, or 1 to change the direction of the search
    // this way, every possible move is considered.
    //
    // dist is for the length of the search
    // dist = 1 searches one slot away, dist = 2 searches 2 slots away
    //
    // cur_x and cur_y are temporary variables to store the temporary
    // positions being searched while looping through the possibilities
    int dir_x, dir_y, dist = 1, cur_x = x, cur_y = y, flip_count = 0;

    // these nested for loops consider -1, 0, and 1 for dir_x and dir_y
    for (dir_x = -1; dir_x < 2; dir_x++) {
        for (dir_y = -1; dir_y < 2; dir_y++) {
            // set initial dist to 1 and increment only if the slot tested before
            // is the opponent's stone
            dist = 1;
            while (true) {
                // dir_x and dir_y only set the direction (-1, 0, or 1)
                cur_x = x + (dist * dir_x);
                cur_y = y + (dist * dir_y);

                // break current loop if it goes off board
                if (cur_x < 0 || cur_y < 0 || cur_x >= size || cur_y >= size)
                    break;

                // break current loop if the slot is blank
                // since no slots after will be able to be taken anyway
                if (gameboard[cur_x][cur_y] == 0)
                    break;

                // if the stone is the current player's stone
                // increment flip count by the distance minus one
                // since the slots preceding the current slot would be flipped
                if (gameboard[cur_x][cur_y] == player) {
                    flip_count += dist - 1;
                    break;
                }

                // break if (dir_x,dir_y) = (0,0) since it'll go into an infinite loop
                if (dir_x == 0 && dir_y == 0)
                    break;
                dist++;
            }
        }
    }
    return flip_count;
}

/**
 * Actually flip slots according to a move
 */
int GameBoard::flip_slots(int x, int y, int player)
{
    assert(x >= 0 && y >= 0);
    assert(x <= size && y <= size);

    // see flip_count... this is the same, except it actually flips slots
    int dir_x, dir_y, dist = 1, cur_x = x, cur_y = y, flip_count = 0;
    for (dir_x = -1; dir_x < 2; dir_x++) {
        for (dir_y = -1; dir_y < 2; dir_y++) {
            dist = 1;
            while (true) {
                cur_x = x + (dist * dir_x);
                cur_y = y + (dist * dir_y);
                if (cur_x < 0 || cur_y < 0 || cur_x >= size || cur_y >= size)
                    break;

                if (gameboard[cur_x][cur_y] == 0)
                    break;

                if (gameboard[cur_x][cur_y] == player) {
                    // backtrace moves and flip opponent's slots
                    // decrement dist and flip until dist is 0
                    for (dist--; dist > 0; dist--) {
                        cur_x = x + (dist * dir_x);
                        cur_y = y + (dist * dir_y);
                        set_slot(cur_x, cur_y, player); // flip slot
                        change_score(-1,(player == 1)?2:1);
                    }
                    break;
                }
                // break if (dir_x,dir_y) = (0,0) since it'll go into an infinite loop
                if (dir_x == 0 && dir_y == 0)
                    break;
                dist++;
            }
        }
    }
    return flip_count;
}

/**
 * Make a move based on a coordinate
 */
bool GameBoard::make_move(int x, int y, int player)
{
    
    //std::cout << "Making move: (" << x + 1 << ", " << y + 1 <<")"<< std::endl;
    assert(x >= 0 && y >= 0);
    assert(x <= size && y <= size);

    if (is_valid_move(x,y,player) == true) {
        set_slot(x,y,player);
        flip_slots(x,y,player);
    //print_board();
        return true;
    }
    assert(false);
    return false;
}

/**
 * Determine if any moves remain
 */
bool GameBoard::moves_exist(int player) const
{
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (get_slot_status(x,y) != 0)
                continue;
            if (flip_count(x,y,player) > 0)
                return true;
        }
    }
    return false;
}

/**
 * Query user to get next move
 */
bool GameBoard::get_move(int player)
{
    int x, y;
    while (true) {
        printf("Player %d's turn: \n",player);
        printf("Input x: ");

        // get x coordinate
        std::cin >> x;
        // sanitate input
        if (std::cin.fail()) {
            std::cin.clear();
            std::string garbage;
            std::getline(std::cin, garbage);
            printf("Invalid input: x value must be an integer.\n");
        }
        else if (x > size || x < 1) {
            std::cin.clear();
            printf("Invalid input: x value but be between 1 and %d.\n",size);
        }
        else {
            // get y coordinate
            printf("Input y: ");
            std::cin >> y;
            // sanitate input
            if (std::cin.fail()) {
                std::cin.clear();
                std::string garbage;
                std::getline(std::cin, garbage);
                printf("Invalid input: y value must be an integer.\n");
            }
            else if (y > size || y < 1) {
                std::cin.clear();
                printf("Invalid input: y must be between 1 and %d.\n",size);
            }
            else {
                bool move = make_move(x-1,y-1,player);
                if (move == false) {
                    printf("Invalid move, try again.\n");
                    continue;
                }
                return true;
            }
        }
    }
}

/**
 * Getter for a player's score
 */
int GameBoard::get_score(int player) const
{
    return score[player];
}

/**
 * Getter for current game status
 */
bool GameBoard::get_status() const
{
    return status;
}


/**
 * Toggles the status of the game.. 0 is game over
 */
void GameBoard::change_status(void)
{
    status = !status;
}

/**
 * getter for number of free slots (0s) on the board
 */
int GameBoard::count_free_slots() const
{
    int count = 0;
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            count += ((get_slot_status(x, y) == 0) ? 1 : 0);
    return count;
}

/**
 * getter for number of slots owned by a player
 */
int GameBoard::count_player_slots(int player) const
{
    int count = 0;
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            count += ((get_slot_status(x, y) == player) ? 1 : 0);
    return count;
}

/**
 * check if a player has won
 */
bool GameBoard::check_win(int player) {
    if (count_free_slots() == 0) {
        change_status();
        return true;
    }
    if (moves_exist((player == 1)?2:1) == 0) {
        change_status();
        return true;
    }
    return false;
}

/**
 * determine who has won the game.
 */
int GameBoard::determine_winner() const
{
    int p1 = count_player_slots(1);
    int p2 = count_player_slots(2);
    if (p1 == p2)
        return 0;
    if (p1 > p2)
        return 1;
    return 2;
}

/**
 * Change a player's score
 */
void GameBoard::change_score(int change, int player)
{
    score[player] += change;
}

/**
 * Print the current board state
 */
void GameBoard::print_board() const
{
    printf("Current board:\n");
    printf("\n\t");
    for (int z = 0; z < size; z++) {
        printf("%d ",z+1);
    }
    printf("\n\n");
    for (int y = 0; y < size; y++) {
        printf("   %d\t",y+1);
        for (int x = 0; x < size; x++) {
            printf("%d ",get_slot_status(x,y));
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Print scores
 */
void GameBoard::print_scores() const
{
    printf("Current stats:\n");
    printf("Player 1 score: %d\n",get_score(1));
    printf("Player 2 score: %d\n\n",get_score(2));
}

/**
 * Free memory on destruction
 */
GameBoard::~GameBoard()
{

    for (int i = 0; i <= size; i++) {
        delete [] gameboard[i];
    }
    delete [] gameboard;
}
