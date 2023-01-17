//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Graph.h"
#include "MiniMax.h"
#include "ToString.h"
#include <iostream>

int main(int argc, char* const argv[])
{
   // Build the game.
   Board board(3, 3);
   Graph graph(board, 0b111);
   MiniMax solver(graph);

   // Initial state.
   auto node = graph.start();
   auto pos = node.position(board);
   auto player = node.player();
   auto move_count = 0;

   // Display the starting board.
   std::cout << "Start" << std::endl;
   std::cout << to_string(board, pos) << std::endl;

   while (!node.is_terminal() && move_count < 100) {
      // Calculate the next move.
      auto next_node = solver.best_move(node, 100 - move_count);
      auto next_pos = next_node.position(board);

      // Output the result.
      std::cout << "Player " << player + 1 << ": "
                << to_string(board, pos[player], next_pos[player]);
      if (next_node.is_terminal()) {
         std::cout << "!";
      }
      std::cout << '\n' << to_string(board, next_pos) << std::endl;

      // Update state.
      node = next_node;
      pos = next_pos;
      player = node.player();
      ++move_count;
   }

   return 0;
}
