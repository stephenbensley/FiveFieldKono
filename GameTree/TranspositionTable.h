#ifndef TranspositionTable_h
#define TranspositionTable_h

#include <optional>
#include <vector>

class Graph;
class Node;

class TranspositionTable
{
public:
   TranspositionTable(const Graph& graph);
   std::optional<int> find(const Node& node) const noexcept;
   void insert(const Node& node, int value) noexcept;

private:
   const Graph& graph_;
   std::vector<char> value_;
};

#endif /* TranspositionTable_h */
