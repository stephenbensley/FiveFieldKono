//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "RepetitionTable.h"
#include "Graph.h"

RepetitionTable::RepetitionTable(const Graph& graph) noexcept
: graph_(graph)
{
   seen_.resize(graph.size());
}

bool RepetitionTable::try_push(const Node& node) noexcept
{
   auto index = graph_.index(node);
   if (seen_[index]) {
      return false;
   }
   seen_[index] = true;
   return true;
}

void RepetitionTable::pop(const Node& node) noexcept
{
   auto index = graph_.index(node);
   assert(seen_[index]);
   seen_[index] = false;
}
