//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "ColorGraph.h"

uint32_t concat(uint16_t upper, uint16_t lower) noexcept
{
   return static_cast<uint32_t>(upper) << 16 |
          static_cast<uint32_t>(lower);
}

bool contains(const ColorNodes& nodes, const ColorNode* node) noexcept
{
   return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
}

ColorGraph::ColorGraph(const Board& board,
                       Color color,
                       ColorPosition start)
: num_pieces_(count_set_bits(start[0]))
{

   auto goal0_bits = start[1];
   auto goal0_cells = board.cells(color, goal0_bits);
   auto goal1_bits = start[0];
   auto goal1_cells = board.cells(color, goal1_bits);

   assert(goal0_cells.size() == num_pieces_);
   assert(goal1_cells.size() == num_pieces_);

   auto positions = build_positions(board, color, goal0_cells, goal1_cells);
   build_nodes(positions, goal0_bits, goal1_bits);
   populate_moves(positions);
   start_index_ = index_[concat(goal1_bits, goal0_bits)];
}

const ColorNode* ColorGraph::node(ColorBitBoard p0,
                                  ColorBitBoard p1) const noexcept
{
   assert((p0 & p1) == 0);
   assert(count_set_bits(p0) == num_pieces_);
   assert(count_set_bits(p1) == num_pieces_);

   auto i = index_.find(concat(p0, p1));
   assert (i != index_.end());
   return &nodes_[i->second];
}

ColorGraph::Positions ColorGraph::build_positions(const Board& board,
                                                  Color color,
                                                  const Cells& goal0,
                                                  const Cells& goal1)
{
   // C(n, k) where n is the number of cells of the given color and k is the
   // number of pieces for each player.
   auto combos = generate_combos(board.num_cells(color),
                                 static_cast<int>(goal0.size()));
   Positions result;
   for (auto& combo : combos) {
      // Collect the cells for the combination.
      Cells cells;
      for (auto index : combo) {
         cells.push_back(board.cell(color, index));
      }

      auto pieces = board.color_bitboard(cells);
      auto reflected = pieces;
      // Can only reflect odd-width boards.
      if (board.width() % 2) {
         reflected = board.color_bitboard(board.reflect_x(cells));
      }
      
      // Add the new Position.
      result.push_back({
         pieces,
         reflected,
         { static_cast<short>(distance(cells, goal0)),
           static_cast<short>(distance(cells, goal1)) },
         board.moves(cells)
      });
   }
   return result;
}

std::pair<uint32_t, uint32_t> ColorGraph::get_keys(const Position& p0,
                                                   const Position& p1) noexcept
{
   return { concat(p0.pieces, p1.pieces), concat(p0.reflected, p1.reflected) };
}

bool ColorGraph::is_valid_combo(const Position& p0, const Position& p1) noexcept
{
   assert(count_set_bits(p0.pieces) == num_pieces_);
   assert(count_set_bits(p1.pieces) == num_pieces_);
   
   // Pieces must not overlap.
   if ((p0.pieces & p1.pieces) != 0) {
      return false;
   }
   // We have two equivalent combinations, so we only need to create a node for
   // one of them. We arbitrarily choose the one with the lowest key.
   auto [p_key, r_key] = get_keys(p0, p1);
   return p_key <= r_key;
}

ColorNode ColorGraph::build_node(uint16_t index,
                                 const Position& p0,
                                 const Position& p1,
                                 ColorBitBoard goal0,
                                 ColorBitBoard goal1)
{
   auto all_pieces = p0.pieces | p1.pieces;
   return {
      index,
      {{
         {
            p0.pieces,
            (p0.pieces & goal0) != 0,
            (all_pieces & goal0) == goal0,
            p0.distance[0]
         },
         {
            p1.pieces,
            (p1.pieces & goal1) != 0,
            (all_pieces & goal1) == goal1,
            p1.distance[1]
         }
      }
   }};
}

void ColorGraph::build_nodes(const Positions& positions,
                             ColorBitBoard goal0,
                             ColorBitBoard goal1)
{
   for (auto p0 : positions) {
      for (auto p1 : positions) {
         if (!is_valid_combo(p0, p1)) {
            continue;;
         }
         // Index is the same as the node's position in the vector.
         auto index = static_cast<int>(nodes_.size());
         nodes_.push_back(build_node(index, p0, p1, goal0, goal1));

         // Index the node both ways, so that both this combo and its
         // reflection map to the same node.
         auto [p_key, r_key] = get_keys(p0, p1);
         index_[p_key] = index;
         index_[r_key] = index;
      }
   }
}

ColorNode* ColorGraph::find(ColorBitBoard p0, ColorBitBoard p1) noexcept
{
   auto i = index_.find(concat(p0, p1));
   assert (i != index_.end());
   return &nodes_[i->second];
}

ColorNodes ColorGraph::build_p0_moves(const Position& p0,
                                      const Position& p1)
{
   ColorNodes result;
   for (auto move : p0.moves) {
      if ((move & p1.pieces) == 0) {
         auto node = find(move, p1.pieces);
         // Since we're consolidating equivalent positions, this move may
         // already be in the vector.
         if (!contains(result, node)) {
            result.push_back(node);
         }
      }
   }
   return result;
}

ColorNodes ColorGraph::build_p1_moves(const Position& p0,
                                      const Position& p1)
{
   ColorNodes result;
   for (auto move : p1.moves) {
       if ((move & p0.pieces) == 0) {
         auto node = find(p0.pieces, move);
         if (!contains(result, node)) {
            result.push_back(node);
         }
      }
   }
   return result;
}

void ColorGraph::build_moves(ColorNode& dst,
                             const Position& p0,
                             const Position& p1)
{
   dst.player[0].moves = build_p0_moves(p0, p1);
   dst.player[1].moves = build_p1_moves(p0, p1);
}

void ColorGraph::populate_moves(const Positions& positions)
{
   for (auto p0 : positions) {
      for (auto p1 : positions) {
         if (is_valid_combo(p0, p1)) {
            build_moves(*find(p0.pieces, p1.pieces), p0, p1);
         }
      }
   }
}

int count_set_bits(uint32_t src) noexcept
{
   // From: https://graphics.stanford.edu/~seander/bithacks.html

   src = src - ((src >> 1) & 0x55555555);
   src = (src & 0x33333333) + ((src >> 2) & 0x33333333);
   return ((src + (src >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}
