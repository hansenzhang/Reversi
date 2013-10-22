#include "Node.hpp"

Node::Node() 
{}
Node::Node(Move m, Node n) 
{
}
Node::~Node() 
{
    
}

void Node::add_child(Move move, Node* n) 
{
   children.insert(std::make_pair(move, n)); 
}

vector<Node*> Node::get_all_children()
{
    vector<Node*> c;
    for (auto it : children) {
        c.push_back(it.second);
    }
    return c;
}

void Node::rank_moves()
{
    //assert(!all_moves.empty());
    int best_val = -12030120, worst_val = 12312323;
    for (auto it : all_moves) {
        if (it.second > best_val) {
            best_val = it.second;
            best_move = it;
        } else if (it.second < worst_val) {
            worst_val = it.second;
            worst_move = it;
        }
    }
}
