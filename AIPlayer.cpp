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

#include "AIPlayer.hpp"

#define CORNER_VALUE 3

#define CORNER_VALUE2 20
#define EDGE_VALUE 10

/**
 *  Constructor for AIPlayer
 */
AIPlayer::AIPlayer(GameBoard* game, int player, int lookahead, int difficulty)
    : player_(player), game_(game), lookahead_(lookahead), diff_(difficulty)
{ }

AIPlayer::~AIPlayer()
{
}

/**
 *  Determine the quality of a move, by giving it a score
 *
 *  This is a pretty weak AI function... we could do a lot better with
 *  lookahead...
 */
int AIPlayer::rate_move(int x, int y, GameBoard* game, int player) const
{
    // return if coordinates are off the board
    if (x >= game->get_size() || y >= game->get_size() || x < 0 || y < 0)
        return 0;

    // return if slot is not empty
    if (game->get_slot_status(x,y) != 0)
        return 0;

    // return if move invalid
    if (game->is_valid_move(x,y,player) == 0)
        return 0;

    // greedy heuristic
    int temp = game->flip_count(x,y,player);

    // favor corners that we take
    if ((x == 0) || (x == (game->get_size() - 1)))
        temp += CORNER_VALUE;
    if ((y == 0) || (y == (game->get_size() - 1)))
        temp += CORNER_VALUE;

    // discourage giving corner to opponent
    if ((x == 1) || (x == (game->get_size() - 2)))
        temp -= CORNER_VALUE;
    if ((y == 1) || (y == (game->get_size() - 2)))
        temp -= CORNER_VALUE;


    // return our score, or 1, whichever is greater
    return (temp > 0) ? temp : 1;
}

int AIPlayer::rate_move_2(int x, int y, GameBoard* game, int player) const
{
    
    if (x >= game->get_size() || y >= game->get_size() || x < 0 || y < 0)
        return 0;

    // return if slot is not empty
    if (game->get_slot_status(x,y) != 0)
        return 0;

    // return if move invalid
    if (game->is_valid_move(x,y,player) == 0)
        return 0;

    // greedy heuristic
    int temp = game->flip_count(x,y,player) * 3;

    // favor corners that we take
    if ((x == 0) || (x == (game->get_size() - 1)))
        temp += CORNER_VALUE;
    if ((y == 0) || (y == (game->get_size() - 1)))
        temp += CORNER_VALUE;

    // discourage giving corner to opponent
    if ((x == 1) || (x == (game->get_size() - 2)))
        temp -= CORNER_VALUE;
    if ((y == 1) || (y == (game->get_size() - 2)))
        temp -= CORNER_VALUE;

    for (int i = 2; i < game->get_size() - 3; i++) {
        if ((x == i && y == 0) || (x == i && y== game->get_size() - 1)) {
           temp += EDGE_VALUE; 
        }
        if ((y == i && x == 0) || (y == i && x== game->get_size() - 1)) {
           temp += EDGE_VALUE; 
        }
        
        if ((x == i && y == 1) || (x == i && y == game->get_size() - 2)) {
            temp -= EDGE_VALUE; 
        }
        if ((y == i && x == 1) || (y == i && x == game->get_size() - 2)) {
            temp -= EDGE_VALUE; 
        }

    }


    // return our score, or 1, whichever is greater
    return (temp > 0) ? temp : 1;
}

/**
 *  Use brute force to evaluate every possible move, and find the best one.
 *  Once the best move is found, play it.
 */
void AIPlayer::play()
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
                temp = rate_move(x, y, game_, player_);
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


Move AIPlayer::alpha_beta(GameBoard* game, int lookahead, int alpha, int beta, int player)
{
    vector<Move> all_moves = get_all_moves(game, player);
    //cout << "Moves are: " << endl;
        //cout << m.x +1  << ", " << m.y + 1 << " Score: " << m.score << endl;
    if (all_moves.empty())
        return Move(-1, -1, maximizing_player(player) * 35);
    if (lookahead == 0) {
        //return heuristic value of previous player
        if (!is_maximizing(player)) {
            Move temp = *(std::max_element(all_moves.begin(), all_moves.end(), [](Move m1, Move m2){return m1.score < m2.score;}));
            //cout << "return: "<< temp.x +1  << ", " << temp.y + 1 << " Score: " << temp.score << endl;
            return temp;
        }
            
        else {
            Move temp =  *(min_element(all_moves.begin(), all_moves.end(), [](Move m1, Move m2){return m1.score < m2.score;}));
            //cout << "return: "<< temp.x +1  << ", " << temp.y + 1 << " Score: " << temp.score << endl;
            return temp;
        }
    }
    Move return_move;
    if (is_maximizing(player)) {
        for (auto m : all_moves) {
            GameBoard temp_board(game);
            temp_board.make_move(m.x, m.y, player);
            Move temp_move = alpha_beta(&temp_board, lookahead - 1, alpha, beta, other_player(player));
            
            if (temp_move.score > alpha) {
                return_move = m;
                alpha = temp_move.score;
            }
            if (alpha >= beta) {
                break;
            }
        }
        return return_move;
    } else {
        for (auto m : all_moves) {
            GameBoard temp_board(game);
            temp_board.make_move(m.x, m.y, player);
            Move temp_move = alpha_beta(&temp_board, lookahead - 1, alpha, beta, other_player(player));
            
            if (temp_move.score < beta) {
                beta = temp_move.score;
                return_move = m;
            }
            if (alpha >= beta) {
                break;
            }
        }
        return return_move;
    }

}

vector<Move> AIPlayer::get_all_moves(GameBoard* game, int player)
{
    vector<Move> all_moves;
    for (int i = 0; i < game->get_size(); i++) {
        for (int j = 0; j < game->get_size(); j++) {
            if (game->is_valid_move(i, j, player)) {
                all_moves.push_back(Move(i, j, maximizing_player(player) * rate_move_2(i, j, game, player)));
            }
        }
    }
    return all_moves;
}

concurrent_vector<Move> AIPlayer::concurrent_get_all_moves(GameBoard* game, int player)
{
    concurrent_vector<Move> all_moves;
    for (int i = 0; i < game->get_size(); i++) {
        for (int j = 0; j < game->get_size(); j++) {
            if (game->is_valid_move(i, j, player)) {
                all_moves.push_back(Move(i, j, maximizing_player(player) * rate_move_2(i, j, game, player)));
            }
        }
    }
    return all_moves;
}

int AIPlayer::other_player(int player) {
    if (player == 1)
        return 2;
    else if (player == 2)
        return 1;

    // should never get here otherwise we have bigger problems
    assert(false);
    return 0;
}

int AIPlayer::maximizing_player(int player)
{
    if (player == player_)
        return 1;
    else
        return -1;
}

bool AIPlayer::is_maximizing(int player)
{
    if (player == player_)
        return true;
    return false;
}
