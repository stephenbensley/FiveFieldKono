//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Strategy.h"
#include <fstream>
#include <limits>

Strategy::Strategy(const Graph& graph)
: graph_(graph)
{
   entries_.resize(graph_.size());
}

Node Strategy::best_move(const Node& from) const noexcept
{
   assert(!from.is_terminal());

   auto color = from.player() ? -1 : +1;

   auto moves = from.moves();
   assert(!moves.empty());
   std::sort(moves.begin(), moves.end(), [](auto lhs, auto rhs) {
      return lhs.distance() < rhs.distance();
   });

   auto best_value = std::numeric_limits<int>::min();
   Node best_move;
   for (auto move : moves) {
      auto move_value = color * find(move).value();
      if (move_value > best_value) {
         best_value = move_value;
         best_move = move;
      }
   }

   assert(!best_move.is_null());
   return best_move;
}

Strategy::Entry Strategy::find(const Node& node) const noexcept
{
   return entries_[graph_.index(node)];
}

Strategy::Entry& Strategy::find(const Node& node) noexcept
{
   return entries_[graph_.index(node)];
}


bool Strategy::load(const char* filename)
{
   std::ifstream istrm(filename, std::ios::binary);
   if (!istrm.is_open()) {
      return false;
   }

   auto bytes = sizeof(Entry) * entries_.size();
   if (!istrm.read(reinterpret_cast<char*>(entries_.data()), bytes)) {
      return false;
   }

   return istrm.peek() == std::istream::traits_type::eof();;
}

void Strategy::save(const char* filename) const noexcept
{
   std::ofstream ostrm(filename, std::ios::binary | std::ios::trunc);
   auto bytes = sizeof(Entry) * entries_.size();
   ostrm.write(reinterpret_cast<const char*>(entries_.data()), bytes);
}
