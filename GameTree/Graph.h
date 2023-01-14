//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef Graph_h
#define Graph_h

#include "ColorGraph.h"
#include "Node.h"

// Represents the game graph.
class Graph
{
public:
   // start0 is the starting location of player 0's pieces. Player 1's start
   // position is simply the reflection of this.
   Graph(const Board& board, BitBoard start0);
   // Starting position of the game.
   Node start() const noexcept;
   // Number of nodes in the graph.
   int size() const noexcept;
   // Densely-packed integer [0,N) that uniquely identifies the node.
   int index(const Node& node) const noexcept;

private:
   ColorGraph black_;
   ColorGraph white_;

};

#endif /* Graph_h */
