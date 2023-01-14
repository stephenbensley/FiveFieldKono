//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"
#include "BitOps.h"

Graph::Graph(const Board& board, BitBoard start0)
: black_(board, BLACK, even_bits(start0)),
  white_(board, WHITE, odd_bits(start0))
{ }

Node Graph::start() const noexcept
{
   // Player 0 always goes first.
   return { 0, black_.start(), white_.start() };
}

int Graph::size() const noexcept
{
   return black_.size() * white_.size();
}

int Graph::index(const Node& node) const noexcept
{
   auto [b_idx, w_idx] = node.indices();
   return b_idx * white_.size() + w_idx;
}
