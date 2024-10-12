//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#include "Strategy.h"
#include "ToString.h"
#include <iostream>

int main(int argc, char* const argv[])
{
   // Build the game and load the strategy.
   Graph graph(5, 5, 0b10001'11111);
   Strategy strategy(graph);
   strategy.load("strategy.dat");

   // Initial state.
   auto node = graph.start();
   auto pos = node.position(graph.board());
   auto player = node.player();
   auto move_count = 0;

   // Display the starting board.
   std::cout << "Start:\n" << to_string(graph.board(), pos) << std::endl;

   // Terminate after 100 moves, so the game doesn't go on forever.
   while (!node.is_terminal() && (move_count < 100)) {
      // Calculate the next move.
      auto next_node = strategy.best_move(node);
      auto next_pos = next_node.position(graph.board());

      // Output the result.
      std::cout << "Player " << player + 1 << ": "
                << to_string(graph.board(), pos[player], next_pos[player]);
      if (next_node.is_terminal()) {
         std::cout << "!";
      }
      std::cout << '\n' << to_string(graph.board(), next_pos) << std::endl;

      // Update state.
      node = next_node;
      pos = next_pos;
      player = node.player();
      ++move_count;
   }

   return 0;
}
