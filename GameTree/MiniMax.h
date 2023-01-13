#ifndef MiniMax_h
#define MiniMax_h

#include "RepetitionTable.h"
#include "TranspositionTable.h"

class Graph;
class Node;

class MiniMax
{
public:
   MiniMax(const Graph& graph);
   
   int search(const Node& node, int depth);
   
private:
   RepetitionTable r_table_;
   TranspositionTable t_table_;

   int negamax(const Node& node, int depth, int color);
};

#endif /* MiniMax_h */
