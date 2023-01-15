//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef TranspositionTable_h
#define TranspositionTable_h

#include <optional>
#include <vector>

class Graph;
class Node;

// Caches the value of previously seen positions.
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
