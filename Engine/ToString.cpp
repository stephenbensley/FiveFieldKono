//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#include "ToString.h"

uint32_t log2(uint32_t x) noexcept;

std::string to_string(const Board& board, const GamePosition& pos)
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

std::string to_string(const Board& board,
                      const BitBoard& from,
                      const BitBoard& to)
{
   auto diff = from ^ to;
   return to_string(board, log2(diff & from)) +
          to_string(board, log2(diff & to));
}

std::string to_string(const Board& board, int ordinal)
{
   auto cell = board.cell(ordinal);
   return { static_cast<char>('a' + cell.x), static_cast<char>('1' + cell.y) };
}

uint32_t log2(uint32_t x) noexcept
{
   uint32_t retval = 0;
   while (x >>= 1) {
      ++retval;
   }
   return retval;
}

