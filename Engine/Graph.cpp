//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"

Graph::Graph(const Board& board, BitBoard start0)
: board_(board),
  num_pieces_(count_set_bits(start0)),
  black_(board, BLACK, get_start_positions(board, start0, BLACK)),
  white_(board, WHITE, get_start_positions(board, start0, WHITE))
{ }

Node Graph::start() const noexcept
{
   // Player 0 always goes first.
   return { 0, black_.start(), white_.start() };
}

Node Graph::node(BitBoard p0, BitBoard p1) const
{
   assert((p0 & p1) == 0);
   assert(count_set_bits(p0) == num_pieces_);
   assert(count_set_bits(p1) == num_pieces_);

   auto p0_black = board_.color_bitboard(BLACK, p0);
   auto p0_white = board_.color_bitboard(WHITE, p0);
   auto p1_black = board_.color_bitboard(BLACK, p1);
   auto p1_white = board_.color_bitboard(WHITE, p1);

   auto black = black_.node(p0_black, p1_black);
   auto white = white_.node(p0_white, p1_white);
   return Node(player(black, white), black, white);

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

Node Graph::operator[](int index) const noexcept
{
   assert(index >= 0);
   assert(index < size());

   auto black = black_[index / white_.size()];
   auto white = white_[index % white_.size()];
   return Node(player(black, white), black, white);
}

int Graph::player(const ColorNode* black, const ColorNode* white) const noexcept
{
   auto parity = (black->parity() + white->parity()) % num_players;
   return (parity == start().parity()) ? 0 : 1;
}

ColorPosition Graph::get_start_positions(const Board& board,
                                         BitBoard start0,
                                         Color color)
{
   auto p0_cells = board.cells(start0);
   auto p1_cells = board.reflect_y(p0_cells);
   return {
      board.color_bitboard(board.filter(p0_cells, color)),
      board.color_bitboard(board.filter(p1_cells, color))
   };
}

