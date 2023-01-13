#include "Node.h"
#include <cassert>

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
   return black().goal_full && white().goal_full &&
         (black().goal_reached || white().goal_reached);
}

