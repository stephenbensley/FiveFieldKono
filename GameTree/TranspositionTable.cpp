#include "TranspositionTable.h"
#include "Graph.h"

TranspositionTable::TranspositionTable(const Graph& graph)
: graph_(graph)
{
   value_.resize(graph.size());
}

std::optional<int> TranspositionTable::find(const Node& node) const noexcept
{
   auto value = value_[graph_.index(node)];
   if (value == 0) {
      return {};
   }
   return value;
}

void TranspositionTable::insert(const Node& node, int value) noexcept
{
   if (value != 0) {
      value_[graph_.index(node)] = value;
   }
}

