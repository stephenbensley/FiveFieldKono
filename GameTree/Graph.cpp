//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"

Graph::Graph(const Board& board, BitBoard start0)
: black_(board, BLACK, board.color_bitboard(BLACK, start0)),
  white_(board, WHITE, board.color_bitboard(WHITE, start0))
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

Node Graph::operator[](int index) const
{
   assert(index >= 0);
   assert(index < size());

   auto black = black_[index / white_.size()];
   auto white = white_[index % white_.size()];

   auto parity = (black->parity() + white->parity()) % num_players;
   auto player = (parity == start().parity()) ? 0 : 1;

   return Node(player, black, white);
}

