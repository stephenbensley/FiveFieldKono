//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "Graph.h"
#include "MiniMax.h"

TEST_CASE("MiniMax::search")
{
   Graph graph(Board(3, 3), 0b111);
   auto result = MiniMax(graph).search(graph.start(), 100);
   CHECK(result == 0);
}
