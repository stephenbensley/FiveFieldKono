#ifndef RepetitionTable_h
#define RepetitionTable_h

#include <vector>

class Graph;
class Node;

class RepetitionTable
{
public:
   RepetitionTable(const Graph& graph) noexcept;
   bool try_push(const Node& node) noexcept;
   void pop(const Node& node) noexcept;

private:
   const Graph& graph_;
   std::vector<bool> seen_;
};

#endif /* RepetitionTable_h */
