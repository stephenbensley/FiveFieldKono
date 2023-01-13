#include "Graph.h"

Graph::Graph(const Board& board, BitBoard start0[num_colors])
: black_(board, BLACK, start0[BLACK]),
  white_(board, WHITE, start0[WHITE])
{ }

int Graph::size() const noexcept
{
   return black_.size() * white_.size();
}

Node Graph::root() const noexcept
{
   return { 0, black_.root(), white_.root() };
}

int Graph::index(const Node& node) const noexcept
{
   auto [b_idx, w_idx] = node.indices();
   return b_idx * white_.size() + w_idx;
}
