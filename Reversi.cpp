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
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "tbb/tick_count.h"
#include "GameBoard.hpp"
#include "AIPlayer.hpp"
#include "AIPlayer_for.hpp"
#include "AIPlayer_fork.hpp"

using namespace std;
using namespace tbb;

/**
 *  Function prototypes
 */
void play(int size, int mode, bool quiet);

/**
 *  Print basic usage
 */
void usage()
{
    printf("Options: -m [1-3] -s[int] -i[1-n] -h\n");
    printf("  -s:  size of the board\n");
    printf("  -m:  game mode\n");
    printf("       1 - two human players\n");
    printf("       2 - one human player\n");
    printf("       3 - zero human players\n");
    printf("  -q:  quiet mode\n");
    printf("  -l:  lookahead of ai (you must have gamemode 2 or 3)\n");
    printf("  -h:  this message\n\n");
    printf("Note: you'll probably want to run with -m 3 -q, and vary -s\n");
}

/**
 * command-line args are "-s" for board size, and "-m" for mode
 */
int main(int argc, char * argv[])
{
    int boardsize = -1;
    int mode = -1;
    bool quiet = false;
    int lookahead = 1;
    int alg = -1;

    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "s:m:l:a:hq")) != -1) {
        switch (opt) {
          case 's': boardsize = atoi(optarg); break;
          case 'm': mode      = atoi(optarg); break;
          case 'q': quiet     = true;         break;
          case 'l': lookahead = atoi(optarg); break;
          case 'h': usage();                  return 0;
          case 'a': alg       = atoi(optarg); break;
        }
    }

    // validate board size
    if (boardsize > 0 && (boardsize % 2) == 0)
        printf("Board size is: %d x %d\n", boardsize, boardsize);
    else {
        usage();
        return 0;
    }

    // validate mode
    if (mode < 1 || mode > 3) {
        usage();
        return 0;
    }

    // validate quiet
    if (mode == 1)
        quiet = false;

    if (lookahead < 1) {
        usage();
        return 0;
    }
    // create game object
    GameBoard game(boardsize);

    
    AIPlayer* ai1 = NULL;
    AIPlayer* ai2 = NULL;
    // create any AI agents
    
    //AIPlayer_for* ai1_f = NULL;
    //AIPlayer_for* ai2_f = NULL;

     
    if (mode == 2) {
        if (alg == 1)
            ai2 = new AIPlayer(&game,2, lookahead, 1);
        else if (alg == 2)
            ai2 = new AIPlayer_for(&game,2, lookahead, 1);
        else    
            ai2 = new AIPlayer_fork(&game,2, lookahead, 1);
    }
    if (mode == 3) {
        if (alg == 1){
            ai1 = new AIPlayer(&game,1, lookahead, 1);
            ai2 = new AIPlayer(&game,2, 0, 0);
        } else if (alg == 2){
            printf("using parallel for\n");
            ai1 = new AIPlayer_for(&game,1, lookahead, 1);
            ai2 = new AIPlayer_for(&game,2, 0, 0);
        } else {
            printf("using parallel fork\n");
            ai1 = new AIPlayer_fork(&game,1, lookahead, 1);
            ai2 = new AIPlayer_fork(&game,2, 0, 0);
        }
    }

    // start main game loop
    if (!quiet) {
        printf("Player 1's turn:\n");
        game.print_board();
    }
    double ai1_time = 0.0, ai2_time = 0.0;
    while (game.get_status() != 0) {
        // get player 1 move, display new state, check for victory
        if (ai1 != NULL) {
            tick_count t0 = tick_count::now();
            ai1->play();
            tick_count t1 = tick_count::now();
            ai1_time += (t1-t0).seconds();
        } else
            game.get_move(1);
        if (!quiet) {
            game.print_board();
            game.print_scores();
        }
        if (game.check_win(1) == true)
            break;
        // repeat procedure for player 2
        if (ai2 != NULL) {
            tick_count t0 = tick_count::now();
            ai2->play();
            tick_count t1 = tick_count::now();
            ai2_time += (t1-t0).seconds();
        } else
            game.get_move(2);
        if (!quiet) {
            game.print_board();
            game.print_scores();
        }
        if (game.check_win(2) == true)
            break;
    }

    // Print final stats
    if (game.get_status() == 0) {
        //game.print_board();
        printf("Game over !\n");
        printf("Player 1 score: %d\n",game.get_score(1));
        printf("Player 2 score: %d\n",game.get_score(2));
        int winner = game.determine_winner();
        if (winner == 0)
            printf("Players have TIED.\n");
        else
            printf("Player %d WINS!\n",winner);
    
        
        printf("Player 1 cpu time: %f\n",ai1_time);
        printf("Player 2 cpu time: %f\n",ai2_time);
    }

    delete ai1;
    ai1 = NULL;
    delete ai2;
    ai2 = NULL;
    return 0;
}
