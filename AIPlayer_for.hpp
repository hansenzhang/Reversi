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

#ifndef AIPLAYER_FOR_HPP__
#define AIPLAYER_FOR_HPP__

#include <atomic>

#include "AIPlayer.hpp"

class breakable_range 
{
    blocked_range<int> range_;
    volatile bool& stop_;
    public:
        breakable_range(int begin, int end, int grainsize, bool stop)
            : range_(begin, end, grainsize), stop_(stop){};

        breakable_range(breakable_range& r , split)
            : range_(r.range_, split()), stop_(r.stop_){};

        void cancel() const
        {
            stop_ = true;
        };

        bool empty() const
        {
            return (stop_ || range_.empty());
        };

        bool is_divisible() const
        {
            return (!stop_ && range_.is_divisible());
        };

        int begin() const { return range_.begin(); };

        int end() const {return stop_ ? range_.begin() : range_.end(); } ;
};


class AIPlayer_for : public AIPlayer
{
  public:
    /**
     *  Constructor just stores a reference to the game, and stores the player
     *  ID
     */
    AIPlayer_for(GameBoard* game, int player, int lookahead, int diff);

    /**
     *  Tell the player to find the best move and make it
     */
    void play();

    Move alpha_beta(GameBoard* game, int lookahead, int alpha, int beta, int player);
    
};

#endif // AIPLAYER_FOR_HPP__
