//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "Graph.h"

TEST_CASE("Graph::size")
{
   // See ColorGraphTest for details on where 16 & 7 came from.
   CHECK(Graph({3,3},0b111).size() == 16 * 7);
}

TEST_CASE("Graph::start")
{
   Graph graph({5,5}, 0b10001'11111);
   auto start = graph.start();

   CHECK(start.player() == 0);
   CHECK(!start.is_terminal());
   // See ColorGraphTest for details about the number of moves from the start
   // position.
   REQUIRE(start.moves().size() == 4);
   CHECK(start.moves()[0].player() == 1);
   CHECK(start.distance() == 24);
}
