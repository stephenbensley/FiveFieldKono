//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#ifndef Retrograde_h
#define Retrograde_h

#include "Strategy.h"

// Performs retrograde analysis to strongly solve the graph.
class Retrograde
{
public:
   Retrograde(const Graph& graph);
   // Solves the graph and returns the value of the starting position.
   int analyze();
   // Returns the strategy generated by a previous call to analyze.
   const Strategy& strategy() const noexcept;
   
private:
   bool analyze_node_zero(const Node& node) noexcept;
   bool analyze_node(const Node& node, int depth) noexcept;
   int analyze_nodes_worker(int index, int depth) noexcept;
   int analyze_nodes(int depth);

   const int num_workers_;
   const int num_nodes_;
   const Graph& graph_;
   Strategy strategy_;
};

inline const Strategy& Retrograde::strategy() const noexcept
{
   return strategy_;
}

#endif /* Retrograde_h */
