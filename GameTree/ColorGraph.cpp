#include "ColorGraph.h"
#include "BitOps.h"

bool contains(const ColorNodes& nodes, const ColorNode* node) noexcept
{
   return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
}

ColorGraph::ColorGraph(const Board& board,
                       Color color,
                       BitBoard start0)
{
   auto goal1_bits = start0;
   auto goal1_cells = board.cells(color, goal1_bits);
   auto goal0_cells = board.reflect_y(goal1_cells);
   auto goal0_bits = board.bitboard(goal0_cells);
   assert((goal0_bits & goal1_bits) == 0);

   auto positions = build_positions(board, color, goal0_cells, goal1_cells);
   build_nodes(positions, goal0_bits, goal1_bits);
   populate_moves(positions);
   root_ = find(goal1_bits, goal0_bits);
}

ColorGraph::Positions ColorGraph::build_positions(const Board& board,
                                                  Color color,
                                                  const Cells& goal0,
                                                  const Cells& goal1)
{
   auto combos = generate_combos(board.num_cells(color),
                                 static_cast<int>(goal0.size()));
   Positions result;
   for (auto& combo : combos) {
      Cells cells;
      for (auto index : combo) {
         cells.push_back(board.cell(color, index));
      }
      result.push_back({
         board.bitboard(cells),
         board.bitboard(board.reflect_x(cells)),
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

bool ColorGraph::is_valid_combo(const Position& p0,
                                const Position& p1) noexcept
{
   if ((p0.pieces & p1.pieces) != 0) {
      return false;
   }

   auto [p_key, r_key] = get_keys(p0, p1);
   return p_key <= r_key;
}

ColorNode ColorGraph::build_node(uint16_t index,
                                 const Position& p0,
                                 const Position& p1,
                                 BitBoard goal0,
                                 BitBoard goal1)
{
   auto all_pieces = p0.pieces | p1.pieces;
   return {
      index,
      {
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
   };
}

void ColorGraph::build_nodes(const Positions& positions,
                             BitBoard goal0,
                             BitBoard goal1)
{
   for (auto p0 : positions) {
      for (auto p1 : positions) {
         if (!is_valid_combo(p0, p1)) {
            continue;;
         }

         auto index = static_cast<int>(nodes_.size());
         nodes_.push_back(build_node(index, p0, p1, goal0, goal1));

         auto [p_key, r_key] = get_keys(p0, p1);
         index_[p_key] = index;
         index_[r_key] = index;
      }
   }
}

ColorNode* ColorGraph::find(BitBoard p0, BitBoard p1) noexcept
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
