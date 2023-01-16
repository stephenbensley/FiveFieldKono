//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "MiniMax.h"
#include "Node.h"
#include <algorithm>
#include <limits>

MiniMax::MiniMax(const Graph& graph)
: r_table_(graph),
  t_table_(graph)
{ }

int MiniMax::search(const Node& node, int depth)
{
   auto color = (node.player() ? -1 : +1);
   return negamax(node, depth, color);
}

int MiniMax::negamax(const Node& node, int depth, int color)
{
   if (depth == 0) {
      return 0;
   }

   if (auto cached = t_table_.find(node)) {
      return *cached;
   }

   if (node.is_terminal()) {
      return evaluate(node);
   }

   if (!r_table_.try_push(node)) {
      return 0;
   }

   auto moves = node.moves();
   std::sort(moves.begin(), moves.end(), [](auto lhs, auto rhs) {
      return lhs.distance() < rhs.distance();
   });

   auto value = std::numeric_limits<int>::min();
   for (auto move : node.moves()) {
      value = std::max(value, -negamax(move, depth - 1, -color));
      if (value == color) {
         break;
      }
   }

   r_table_.pop(node);
   t_table_.insert(node, value);
   return value;
}

int MiniMax::evaluate(const Node& node) noexcept
{
   assert(node.is_terminal());
   if (node.is_winner(0)) {
      return +1;
   }
   if (node.is_winner(1)) {
      return -1;
   }
   if (node.no_moves()) {
      return node.player() ? -1 : + 1;
   }
   return 0;
}
