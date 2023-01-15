//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "TranspositionTable.h"
#include "Graph.h"

TEST_CASE("TranspositionTable::find/insert")
{
   Graph graph({3,3}, 0b111);
   TranspositionTable t_table(graph);
   auto node = graph.start();

   CHECK(!t_table.find(node));
   t_table.insert(node, +1);
   CHECK(t_table.find(node) == +1);
   t_table.insert(node, 0);
   CHECK(!t_table.find(node));
}
