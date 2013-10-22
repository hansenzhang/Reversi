
Move AIPlayer::find_best_move(GameBoard* game, int player, int lookahead, int alpha, int beta)
{
    
    Move return_move(-1, -1);
    return_move.score = (maximizing_player(player)) * (std::numeric_limits<int>::max() -64); // Negative infinity
    for (int row = 0; row < game->get_size(); row++) {
        for (int col = 0; col < game->get_size(); col++) {
            if (game->is_valid_move(row, col, player)) {
                Move temp_move(row, col);
                GameBoard* temp_board = new GameBoard(game);
                temp_board->make_move(temp_move.x, temp_move.y, player);
                int local_score = temp_board->get_score(other_player(player_)) - temp_board->get_score((player_)); 
                if (lookahead == 0) {
                    // End recursion and return base score of the gameboard
                    //TODO: add modifier if other player doesn't have any moves
                    temp_move.score = local_score * 0 +  1 * rate_move_2(row, col, game, player); 
                    cout << "terminal node"  << " (" << temp_move.x +1 << ", " << temp_move.y+1 << ")->" << temp_move.score << endl;
                } else {
                    std::string maximize = "";
                    if (is_maximizing(player)){
                        maximize = "Maximize";
                    } else
                        maximize = "Minimize";
                    
                    Move next_move = find_best_move(temp_board, other_player(player), lookahead - 1, alpha, beta);
                    temp_move.score = next_move.score;
                    // adjust alpha beta after recursion
                    cout << maximize + " lookahead: " << lookahead << " (" << temp_move.x+1 << ", " << temp_move.y+1 << ")->" << temp_move.score << endl << endl;
                    if (!is_maximizing(player) && temp_move.score > alpha) 
                        alpha = temp_move.score;
                    if (is_maximizing(player) && temp_move.score < beta)
                        beta = temp_move.score;
                }
                if (!is_maximizing(player) && temp_move.score > beta) {
                    temp_move.score = beta;
                    delete temp_board;
                    cout << "Return: " << " (" << temp_move.x+1 << ", " << temp_move.y+1 << ")->" << temp_move.score << endl;
                    return temp_move;
                } 
                if (is_maximizing(player) && temp_move.score < alpha) {
                    temp_move.score = alpha;
                    delete temp_board;
                    cout << "Return: " << " (" << temp_move.x+1 << ", " << temp_move.y+1 << ")->" << temp_move.score << endl;
                    return temp_move;
                } 
                
                if (return_move.x < 0)
                    return_move = temp_move;
                else if (maximizing_player(player) * temp_move.score > maximizing_player(player) * return_move.score)
                    return_move = temp_move;
                delete temp_board;
            }
        }
    }
    
    return return_move;

}
