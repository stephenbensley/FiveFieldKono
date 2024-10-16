//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#ifndef Node_h
#define Node_h

#include "ColorGraph.h"

// Represents a node in the game graph.
class Node
{
public:
   Node() = default;
   Node(int player, const ColorNode* black, const ColorNode* white) noexcept;
   // The player with the next move.
   int player() const noexcept;
   // The indices of the black & white ColorNodes.
   std::pair<uint16_t, uint16_t> indices() const noexcept;
   // Location of pieces.
   GamePosition position(const Board& board) const noexcept;
   // Returns true if this node was default constructed. Useful for detecting
   // nodes that haven't been initialized to an actual game node.
   bool is_null() const noexcept;
   // Return true if the game is over once this node is reached.
   bool is_terminal() const noexcept;
   // Returns true if the current player has no moves available. This is much
   // faster than moves().empty().
   bool no_moves() const noexcept;
   // Returns true if the specified player has won the game by reaching and
   // filling their goal.
   bool is_winner(int idx) const noexcept;
   // Available moves for the current player.
   std::vector<Node> moves() const;
   // Number of moves it would take the current player to put all his pieces
   // on the goal if the other player doesn't interfere.
   int distance() const noexcept;
   // The parity changes whenever a move is made. The parity of the node is
   // useful for determining whose turn it is.
   int parity() const noexcept;

   bool operator==(const Node& rhs) const noexcept;

private:
   const ColorNode::Player& black() const noexcept;
   const ColorNode::Player& white() const noexcept;

   int player_ = 0;
   const ColorNode* black_ = nullptr;
   const ColorNode* white_ = nullptr;
};

inline int Node::player() const noexcept
{
   return player_;
}

inline std::pair<uint16_t, uint16_t> Node::indices() const noexcept
{
   return { black_->index, white_->index };
}

inline bool Node::is_null() const noexcept
{
   return black_ == nullptr;
}

inline int Node::parity() const noexcept
{
   return (black_->parity() + white_->parity()) % num_players;
}

#endif /* Node_h */
