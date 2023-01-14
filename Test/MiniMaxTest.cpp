#include "catch.hpp"
#include "Graph.h"
#include "MiniMax.h"

TEST_CASE("MiniMax::search")
{
   Graph graph(Board(3, 3), 0b111);
   auto result = MiniMax(graph).search(graph.root(), 10);
   CHECK(result == +1);
}
