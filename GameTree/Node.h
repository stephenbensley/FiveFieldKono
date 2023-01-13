#ifndef Node_h
#define Node_h

#include "ColorGraph.h"

class Node
{
public:
   Node(int player, const ColorNode* black, const ColorNode* white) noexcept;
   int player() const noexcept;
   std::pair<uint16_t, uint16_t> indices() const noexcept;
   bool is_terminal() const noexcept;
   int value() const noexcept;
   std::vector<Node> moves() const;
   int distance() const noexcept;

private:
   const ColorNode::Player& black() const noexcept;
   const ColorNode::Player& white() const noexcept;

   bool no_moves() const noexcept;
   bool is_winner(int idx) const noexcept;

   int player_;
   const ColorNode* black_;
   const ColorNode* white_;
};

inline int Node::player() const noexcept
{
   return player_;
}

inline std::pair<uint16_t, uint16_t> Node::indices() const noexcept
{
   return { black_->index, white_->index };
}

#endif /* Node_h */
