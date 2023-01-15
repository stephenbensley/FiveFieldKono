//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef MiniMax_h
#define MiniMax_h

#include "RepetitionTable.h"
#include "TranspositionTable.h"

class Graph;
class Node;

// Uses the MiniMax algorithm to search a game position.
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
