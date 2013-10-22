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

#ifndef AIPLAYER_FORK_HPP__
#define AIPLAYER_FORK_HPP__

#include "AIPlayer.hpp"

class AIPlayer_fork : public AIPlayer
{
  public:
    /**
     *  Constructor just stores a reference to the game, and stores the player
     *  ID
     */
    AIPlayer_fork(GameBoard* game, int player, int lookahead, int diff);

    /**
     *  Tell the player to find the best move and make it
     */
    void play();
    
    Move alpha_beta(GameBoard* game, int lookahead, int alpha, int beta, int player);

};

#endif // AIPLAYER_FOR_HPP__
