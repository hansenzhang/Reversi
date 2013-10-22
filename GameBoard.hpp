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

#ifndef GAMEBOARD_HPP__
#define GAMEBOARD_HPP__

class GameBoard
{
    // the matrix representing the board
    int** gameboard;

    // the size of the board
    int size;

    // for tracking score
    int score[3];
    bool status;

    void change_status();
    void set_slot(int x, int y, int player);
    int flip_slots(int x, int y, int player);
    void change_score(int change, int player);
    bool moves_exist(int player) const;
    int count_free_slots() const;
    int count_player_slots(int player) const;

  public:
    GameBoard(int size = 8);
    GameBoard(GameBoard* g);
    virtual ~GameBoard();
    int get_size() const;
    int get_slot_status(int x, int y) const;
    bool is_valid_move(int x, int y, int player) const;
    int flip_count(int x, int y, int player) const;
    bool make_move(int x, int y, int player);
    void print_board() const;
    bool get_status() const;
    bool get_move(int player);
    bool check_win(int player);
    void print_scores() const;
    int get_score(int player) const;
    int determine_winner() const;
};

#endif // GAMEBOARD_HPP__
