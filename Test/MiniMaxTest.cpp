#include "Catch.hpp"
#include "Graph.h"
#include "MiniMax.h"

TEST_CASE("MiniMax::search")
{
   BitBoard start0[] = { 0b11, 0b1 };
   Graph graph(Board(3, 3), start0);
   auto result = MiniMax(graph).search(graph.root(), 10);
   CHECK(result == +1);
}
