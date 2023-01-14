#ifndef Graph_h
#define Graph_h

#include "ColorGraph.h"
#include "Node.h"

class Graph
{
public:
   Graph(const Board& board, uint32_t start0);
   int size() const noexcept;
   Node start() const noexcept;
   int index(const Node& node) const noexcept;

private:
   ColorGraph black_;
   ColorGraph white_;

};

#endif /* Graph_h */
