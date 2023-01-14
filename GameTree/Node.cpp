//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Node.h"

Node::Node(int player, const ColorNode* black, const ColorNode* white) noexcept
: player_(player),
  black_(black),
  white_(white)
{
   assert(is_valid_player(player));
   assert(black != nullptr);
   assert(white != nullptr);
}

bool Node::is_terminal() const noexcept
{
   return no_moves() || is_winner(0) || is_winner(1);
}

int Node::value() const noexcept
{
   assert(is_terminal());
   if (is_winner(0)) {
      return +1;
   }
   if (is_winner(1)) {
      return -1;
   }
   if (no_moves()) {
      return player() ? -1 : + 1;
   }
   return 0;
}

std::vector<Node> Node::moves() const
{
   auto next = other_player(player());

   // Player can move a black piece or a white piece.
   std::vector<Node> result;
   for (auto move : black().moves) {
      result.push_back(Node(next, move, white_));
   }
   for (auto move : white().moves) {
      result.push_back(Node(next, black_, move));
   }
   return result;
}

int Node::distance() const noexcept
{
   return black().distance + white().distance;
}

const ColorNode::Player& Node::black() const noexcept
{
   return black_->player[player()];
}

const ColorNode::Player& Node::white() const noexcept
{
   return white_->player[player()];
}

bool Node::no_moves() const noexcept
{
   return black().moves.empty() && white().moves.empty();
}

bool Node::is_winner(int idx) const noexcept
{
   // Player's goal must be full and at least one of the pieces in the goal
   // must belong to the player.
   return black().goal_full && white().goal_full &&
         (black().goal_reached || white().goal_reached);
}

