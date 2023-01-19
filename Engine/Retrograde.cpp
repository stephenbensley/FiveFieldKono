//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Retrograde.h"
#include "ToString.h"
#include <future>

Retrograde::Retrograde(const Graph& graph)
: num_workers_(std::thread::hardware_concurrency()),
  num_nodes_(graph.size()),
  graph_(graph),
  strategy_(graph)
{ }

int Retrograde::analyze()
{
   for (auto depth = 0; depth < strategy_.max_depth(); ++depth) {
      auto count = analyze_nodes(depth);
      // If no nodes were updated, we can't make any more progress.
      if (count == 0) {
         break;
      }
   }

   return strategy_.find(graph_.start()).value();
}

bool Retrograde::analyze_node_zero(const Node& node) noexcept
{
   // Retrieve the entry for this node.
   auto& entry = strategy_.find(node);

   if (node.is_winner(0)) {
      entry = { 0, 0 };
   } else if (node.is_winner(1)) {
      entry = { 1, 0 };;
   } else if (node.no_moves()) {
      entry = { other_player(node.player()), 0 };
   } else {
      // Not a terminal node, so ignore on pass zero.
      return false;
   }

   return true;
}

bool Retrograde::analyze_node(const Node& node, int depth) noexcept
{
   // Retrieve the entry for this node.
   auto& entry = strategy_.find(node);

   // If the entry has already been assigned a value, there's nothing to do.
   if (!entry.empty()) {
      return false;
   }

   // Number of moves that lead to a guaranteed loss.
   auto loss_count = 0;

   auto moves = node.moves();
   assert(moves.size() != 0);
   for (auto move : moves) {
      Strategy::Entry move_entry = strategy_.find(move);

      // Ignore empty entries.
      if (move_entry.empty()) {
         continue;
      }

      // If we find even one winner, the player can always make that move, so
      // this node is also a guaranteed winner.
      if (move_entry.winner() == node.player()) {
         entry = { node.player(), depth };
         return true;
      }

      ++loss_count;
   }

   // If every node leads to a guaranteed loss, then there's nothing the
   // current player can do to avoid it, so this node is a guaranteed loss, too.
   if (loss_count == moves.size()) {
      entry = { other_player(node.player()), depth };
      return true;
   }

   return false;
}

int Retrograde::analyze_nodes_worker(int index, int depth) noexcept
{
   auto count = 0;
   for (auto i = index; i < num_nodes_; i += num_workers_) {
      auto node = graph_[i];
      if ((depth == 0) ? analyze_node_zero(node) : analyze_node(node, depth)) {
         ++count;
      }
   }
   return count;
}

int Retrograde::analyze_nodes(int depth)
{
   // Launch the workers ...
   std::vector<std::future<int>> futures;
   for (auto i = 0; i < num_workers_; ++i) {
      futures.push_back(std::async(std::launch::async,
                                   &Retrograde::analyze_nodes_worker,
                                   this,
                                   i,
                                   depth));
   }
   // ... and wait for them to complete.
   auto count = 0;
   std::for_each(futures.begin(), futures.end(), [&count](auto& f){
      count += f.get();
   });

   return count;
}
