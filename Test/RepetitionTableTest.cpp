//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "RepetitionTable.h"
#include "Graph.h"

TEST_CASE("RepetitionTable::try_push/pop")
{
   Graph graph({3,3}, 0b111);
   RepetitionTable r_table(graph);
   auto node = graph.start();

   CHECK(r_table.try_push(node));
   CHECK(!r_table.try_push(node));
   CHECK(!r_table.try_push(node));
   r_table.pop(node);
   CHECK(r_table.try_push(node));
}

