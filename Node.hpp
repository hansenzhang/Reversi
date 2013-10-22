#ifndef NODE_HPP__
#define NODE_HPP__

#include <iostream>
#include <vector>
#include <map>


using std::map;
using std::pair;
using std::vector;

typedef pair<pair<int, int>,int> Move;

/*
namespace std {
    template<>
    struct hash<Move>
    {
        std::size_t operator()(const Move& k) const
        {
        using std::size_t;
        using std::hash;
        //primative hash function
        return (hash<int>()(k.first.first)) ^ (hash<int>()((k.first.second + 1) << 1) >> 1);
        };
    };
};
*/

class Node {
    public:
        // public members...
        vector<Move> all_moves;
        map<Move, Node*> children;
        Move worst_move, best_move;
        int score;
        // public methods
        Node();
        Node(Move p, Node n);
        ~Node();
        void add_child(Move move, Node* n);
        vector<Node*> get_all_children(); 
        void rank_moves();
};

#endif //NODE_HPP__
