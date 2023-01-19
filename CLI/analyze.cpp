//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"
#include "Retrograde.h"
#include "ToString.h"

#include <iostream>

int main(int argc, char* const argv[])
{
   Graph graph(5, 5, 0b10001'11111);
   Retrograde retro(graph);
   auto value = retro.analyze();
   std::cout << "Value of start position: " << value << std::endl;
   retro.strategy().save("strategy.dat");
   return 0;
}
