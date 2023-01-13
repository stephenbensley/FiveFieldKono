#ifndef Graph_h
#define Graph_h

#include "ColorGraph.h"
#include "Node.h"

class Graph
{
public:
   Graph(const Board& board, BitBoard start0[num_colors]);
   int size() const noexcept;
   Node root() const noexcept;
   int index(const Node& node) const noexcept;

private:
   ColorGraph black_;
   ColorGraph white_;

};

#endif /* Graph_h */
