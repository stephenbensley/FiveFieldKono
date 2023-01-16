//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"
#include "ToString.h"
#include <iostream>

int main(int argc, char* const argv[])
{
   Board board(5, 5);
   Graph graph(board, 0b10001'11111);
   std::cout << to_string(board, graph.start().position(board)) << std::flush;
   return 0;
}
