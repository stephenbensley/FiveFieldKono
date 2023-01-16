//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "ToString.h"

std::string to_string(const Board& board,
                      const GamePosition& pos)
{
   constexpr char empty_cell = '-';
   constexpr char p0_cell = 'X';
   constexpr char p1_cell = 'O';

   std::string result;
   for (auto row = board.height() - 1; row >= 0; --row) {
      for (auto col = 0; col < board.width(); ++col) {
         auto bit = 1 << board.ordinal({col, row});
         if (pos[0] & bit) {
            result.push_back(p0_cell);
         } else if (pos[1] & bit) {
            result.push_back(p1_cell);
         } else {
            result.push_back(empty_cell);
         }
      }
      result.push_back('\n');
   }

   return result;
}
