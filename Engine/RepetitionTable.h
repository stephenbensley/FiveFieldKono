//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef RepetitionTable_h
#define RepetitionTable_h

#include <vector>

class Graph;
class Node;

// Used to detect repetitions in a sequence of play.
class RepetitionTable
{
public:
   RepetitionTable(const Graph& graph) noexcept;
   // If the node has already been visited, returns false and does not push
   // the node onto the stack. If this function succeeds, you must call pop.
   bool try_push(const Node& node) noexcept;
   // Undoes a successful call to try_push.
   void pop(const Node& node) noexcept;

private:
   const Graph& graph_;
   std::vector<bool> seen_;
};

#endif /* RepetitionTable_h */
