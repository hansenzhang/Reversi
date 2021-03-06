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


#include "AIPlayer_for.hpp"

/**
 *  Constructor for AIPlayer
 */
AIPlayer_for::AIPlayer_for(GameBoard* game, int player, int lookahead, int difficulty)
    : AIPlayer(game, player, lookahead, difficulty){ }

/**
 *  Determine the quality of a move, by giving it a score
 *
 *  This is a pretty weak AI function... we could do a lot better with
 *  lookahead...
 */
void AIPlayer_for::play()
{
    int x = 0,
        y = 0,
        cur_max = 0,
        best_x = 0,
        best_y = 0,
        temp = 0,
        rand_n = 0;
    if (diff_ == 1){
        Move best_move = alpha_beta(game_, lookahead_, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), player_);
        best_x = best_move.x;
        best_y = best_move.y;
    }
    else{    
        for (x = 0; x < game_->get_size(); x++) {
            for (y = 0; y < game_->get_size(); y++) {
                temp = rate_move_2(x, y, game_, player_);
                // if this move is better, choose it
                if (temp > cur_max) {
                    cur_max = temp;
                    best_x = x;
                    best_y = y;
                }
                // otherwise flip a coin to make a decision
                else if (temp == cur_max) {
                    rand_n = rand() % 2;
                    if (rand_n  == 1) {
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
    }
    //cout << "Making Move: " << best_x + 1  << ", " << best_y + 1 << endl;
    game_->make_move(best_x, best_y, player_);  
}


Move AIPlayer_for::alpha_beta(GameBoard* game, int lookahead, int alpha, int beta, int player)
{
    concurrent_vector<Move> all_moves = concurrent_get_all_moves(game, player);
    ///cout << "Moves are: " << endl;
    //for (Move m : all_moves)    
        //printf("%d, %d Score: %d\n", m.x + 1, m.y + 1, m.score);
        //cout << m.x +1  << ", " << m.y + 1 << " Score: " << m.score << endl;
    if (all_moves.empty())
        return Move(-1, -1, maximizing_player(player) * 35);
    if (lookahead == 0) {
        //return heuristic value of previous player
        if (!is_maximizing(player)) {
            Move temp = *(std::max_element(all_moves.begin(), all_moves.end(), [](Move m1, Move m2){return m1.score < m2.score;}));
            
            //printf("return: %d, %d Score: %d\n", temp.x + 1, temp.y + 1, temp.score + 1);
            //cout << "return: "<< temp.x +1  << ", " << temp.y + 1 << " Score: " << temp.score << endl;
            return temp;
        }
            
        else {
            Move temp =  *(std::min_element(all_moves.begin(), all_moves.end(), [](Move m1, Move m2){return m1.score < m2.score;}));
            //printf("return: %d, %d Score: %d\n", temp.x + 1, temp.y + 1, temp.score + 1);
            //cout << "return: "<< temp.x +1  << ", " << temp.y + 1 << " Score: " << temp.score << endl;
            return temp;
        }
    }
    Move return_move;
    if (is_maximizing(player)) {
        //std::atomic<bool> stop (false);
        parallel_for(blocked_range<int>(0, all_moves.size()),
                [&] (blocked_range<int> &r) {        
                for (int i = r.begin(); i != r.end(); i++) {   
                GameBoard temp_board(game);
                    temp_board.make_move(all_moves[i].x, all_moves[i].y, player);
                    Move temp_move = alpha_beta(&temp_board, lookahead - 1, alpha, beta, other_player(player));
            
                    if (temp_move.score > alpha) {
                        return_move = all_moves[i];
                        alpha = temp_move.score;
                    }
                    if (alpha >= beta) {
                        break;
                        }
                    }
                }
            );
        return return_move;
    } else {
        parallel_for(blocked_range<int>(0, all_moves.size()),
                [&] (blocked_range<int> &r) {        
                for (int i = r.begin(); i != r.end(); i++) {   
            GameBoard temp_board(game);
            temp_board.make_move(all_moves[i].x, all_moves[i].y, player);
            Move temp_move = alpha_beta(&temp_board, lookahead - 1, alpha, beta, other_player(player));
            
            if (temp_move.score < beta) {
                beta = temp_move.score;
                return_move = all_moves[i];
            }
            if (alpha >= beta) {
                break;
                }
        }
    
        });
        return return_move;
    }
}


