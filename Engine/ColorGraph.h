//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#ifndef ColorGraph_h
#define ColorGraph_h

#include "Board.h"
#include <cassert>
#include <unordered_map>

bool is_valid_player(int player) noexcept;
int other_player(int player) noexcept;

class ColorNode;
using ColorNodes = std::vector<const ColorNode*>;

// Represents a node in the per-color graph of the game, i.e., either the
// black or the white graph.
struct ColorNode {
   struct Player {
      // Locations of the player's pieces, where the bit corresponds to the
      // ordinal.
      ColorBitBoard pieces;
      // True if at least one of the player's pieces is currently on a goal
      // cell.
      bool goal_reached;
      // True if all the player's goal cells are occupied by any player.
      bool goal_full;
      // Number of moves required for the player to move all his pieces to
      // the goal, provided they are not blocked by the opponent.
      short distance;
      // Valid moves for the player assuming none is blocked by the opponent.
      ColorNodes moves;
   };

   // Densely-packed integer [0,N) that uniquely identifies the node.
   uint16_t index;
   // Each player's state for the node.
   std::array<Player, num_players> player;
   // The parity changes whenever a move is made. The parity of the node is
   // useful for determining whose turn it is.
   int parity() const noexcept;
};

// Represents the game graph for a given color
class ColorGraph
{
public:
   ColorGraph(const Board& board, Color color, ColorPosition start);

   // Starting position of the game.
   const ColorNode* start() const noexcept;
   // Returns any node based on the piece positions.
   const ColorNode* node(ColorBitBoard p0, ColorBitBoard p1) const noexcept;
   // Number of nodes in the graph.
   int size() const noexcept;
   // Returns the node at the given index.
   const ColorNode* operator[](int index) const noexcept;

private:
   // Used to store intermediate state about a position during graph
   // construction.
   struct Position {
      // Location of the pieces.
      ColorBitBoard pieces;
      // Location of the pieces reflected horizontally.
      ColorBitBoard reflected;
      // Number of moves to the goal.
      std::array<short, num_players> distance;
      // Valid moves from this position.
      ColorBitBoards moves;
   };
   using Positions = std::vector<Position>;

   // Builds a vector of all valid Positions.
   static Positions build_positions(const Board& board,
                                    Color color,
                                    const Cells& goal0,
                                    const Cells& goal1);
   // Returns keys uniquely identifying the combination and the reflection of
   // the combination.
   static std::pair<uint32_t, uint32_t> get_keys(const Position& p0,
                                                 const Position& p1) noexcept;
   // Returns true if the combination of the two positions can occur during
   // game play and isn't simply a reflection of another position.
   bool is_valid_combo(const Position& p0, const Position& p1) noexcept;
   // Populates all the fields in a ColorNode struct except the moves.
   static ColorNode build_node(uint16_t index,
                               const Position& p0,
                               const Position& p1,
                               ColorBitBoard goal0,
                               ColorBitBoard goal1);
   // Builds all the ColorNodes that can be formed by combining positions.
   void build_nodes(const Positions& positions,
                    ColorBitBoard goal0,
                    ColorBitBoard goal1);
   // Returns the ColorNode corresponding to the specified positions.
   ColorNode* find(ColorBitBoard p0, ColorBitBoard p1) noexcept;
   // Builds player 0's moves for the combo.
   ColorNodes build_p0_moves(const Position& p0,
                             const Position& p1);
   // Builds player 1's moves for the combo.
   ColorNodes build_p1_moves(const Position& p0,
                             const Position& p1);
   // Builds both players moves for the combo.
   void build_moves(ColorNode& dst,
                    const Position& p0,
                    const Position& p1);
   // Iterates through all the ColorNodes and initializes there moves field.
   void populate_moves(const Positions& positions);

   // Number of pieces for each player.
   int num_pieces_;
   // All the nodes in the graph in index order, so a ColorNode's index
   // represents its position in the vector.
   std::vector<ColorNode> nodes_;
   // Maps keys to indices.
   std::unordered_map<uint32_t, int> index_;
   // Starting node of the game.
   int start_index_;
};

int count_set_bits(uint32_t src) noexcept;

inline int ColorNode::parity() const noexcept
{
   return (player[0].distance + player[1].distance) % num_players;
}

inline const ColorNode* ColorGraph::start() const noexcept
{
   return operator[](start_index_);
}

inline int ColorGraph::size() const noexcept
{
   return static_cast<int>(nodes_.size());
}

inline const ColorNode* ColorGraph::operator[](int index) const noexcept
{
   assert(index >= 0);
   assert(index < size());
   return &nodes_[index];
}

inline bool is_valid_player(int player) noexcept
{
   return (player >= 0) && (player < num_players);
}

inline int other_player(int player) noexcept
{
   return player ? 0 : 1;
}

#endif /* ColorGraph_h */
