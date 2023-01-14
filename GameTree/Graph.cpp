#include "Graph.h"
#include "BitOps.h"

Graph::Graph(const Board& board, BitBoard start0)
: black_(board, BLACK, even_bits(start0)),
  white_(board, WHITE, odd_bits(start0))
{ }

int Graph::size() const noexcept
{
   return black_.size() * white_.size();
}

Node Graph::start() const noexcept
{
   return { 0, black_.start(), white_.start() };
}

int Graph::index(const Node& node) const noexcept
{
   auto [b_idx, w_idx] = node.indices();
   return b_idx * white_.size() + w_idx;
}
