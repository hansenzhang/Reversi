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

#ifndef AIPLAYER_HPP__
#define AIPLAYER_HPP__

#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <limits>
#include <algorithm>

#include "tbb/tbb.h"
#include "GameBoard.hpp"

using std::cout;
using std::endl;
using std::vector;

using namespace tbb;

struct Move
{
    int x;
    int y;
    int score;
    Move(int i, int j) : x(i), y(j) {}; 
    Move(int i, int j, int score) : x(i), y(j), score(score) {}; 
    Move(){};
};

class AIPlayer
{
  public:
    /**
     *  ID of the player
     */
    int player_;

    /**
     *  Reference to game board
     */
    GameBoard* game_;

    /**
     *  How far ahead our AI should look for optimal moves
     */
    int lookahead_;
    
    int diff_;
    /**
     *  Internal method to score a move
     */
    virtual int rate_move(int x, int y, GameBoard* g, int player) const;
    virtual int rate_move_2(int x, int y, GameBoard* g, int player) const;
    /**
     *  Constructor just stores a reference to the game, and stores the player
     *  ID
     */
    AIPlayer(GameBoard* game, int player, int lookahead, int diff);

    virtual ~AIPlayer();

    /**
     *  Tell the player to find the best move and make it
     */
    virtual void play();

    virtual Move alpha_beta(GameBoard* game, int lookahead, int alpha, int beta, int player);

    virtual vector<Move> get_all_moves(GameBoard* game, int player);
    virtual concurrent_vector<Move> concurrent_get_all_moves(GameBoard* g, int player);

    virtual int other_player(int player);

    virtual int maximizing_player(int player);
    virtual bool is_maximizing(int player);
};

#endif // AIPLAYER_HPP__
