#include "Graph.h"

uint16_t even_bits(uint32_t src) noexcept
{
   uint16_t dst = 0;
   uint32_t src_bit = 1;
   uint16_t dst_bit = 1;
   do {
      if (src & src_bit) {
         dst |= dst_bit;
      }
      src_bit <<= 2;
      dst_bit <<= 1;
   } while (src_bit != 0);

   return dst;
}

uint16_t odd_bits(uint32_t src) noexcept
{
   return even_bits(src >> 1);
}

Graph::Graph(const Board& board, uint32_t start0)
: black_(board, BLACK, even_bits(start0)),
  white_(board, WHITE, odd_bits(start0))
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
