//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "Board.h"
#include <algorithm>
#include <cassert>
#include <limits>

bool contains(const BitBoards& boards, BitBoard board) noexcept
{
   return std::find(boards.begin(), boards.end(), board) != boards.end();
}

Cells Cell::neighbors() const
{
   Cells result;
   result.push_back({ x - 1, y - 1 });
   result.push_back({ x - 1, y + 1 });
   result.push_back({ x + 1, y - 1 });
   result.push_back({ x + 1, y + 1 });
   return result;
}

bool operator<(const Cell& lhs, const Cell& rhs) noexcept
{
   // Compare y first, so that ordering matches ordering by ordinal.
   return (lhs.y < rhs.y) ||
          ((lhs.y == rhs.y) && (lhs.x < rhs.x));
}

bool operator==(const Cell& lhs, const Cell& rhs) noexcept
{
   return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator==(const Cells& lhs, const Cells& rhs) noexcept
{
   if (lhs.size() != rhs.size()) {
      return false;
   }

   for (auto cell : lhs) {
      if (!contains(rhs, cell)) {
         return false;
      }
   }

   return true;
}

bool contains(const Cells& cells, const Cell& cell) noexcept
{
   return std::find(cells.begin(), cells.end(), cell) != cells.end();
}

int distance(const Cell& lhs, const Cell& rhs) noexcept
{
   // Since pieces move diagonally, each move changes both the x and y
   // coordinates.
   return std::max(std::abs(lhs.x - rhs.x), std::abs(lhs.y - rhs.y));
}

int distance(const Cells& lhs, const Cells& rhs) noexcept
{
   assert(lhs.size() == rhs.size());

   // Set of indices to permute.
   std::vector<int> indices;
   // 2D matrix of distances from each cell on the lhs to each cell on the rhs.
   std::vector<std::vector<int>> distances(lhs.size());

   for (auto i = 0; i < lhs.size(); ++i) {
      indices.push_back(i);
      for (auto j = 0; j < rhs.size(); ++j) {
         distances[i].push_back(distance(lhs[i], rhs[j]));
      }
   }

   // Iterate through all permutations and find out which one yields the
   // shortest total distance.
   auto result = std::numeric_limits<int>::max();
   do {
      auto total = 0;
      for (auto i = 0; i < lhs.size(); ++i) {
         total += distances[i][indices[i]];
      }
      result = std::min(result, total);
   } while (std::next_permutation(indices.begin(), indices.end()));

   return result;
}

Board::Board(int width, int height) noexcept
: width_(width), height_(height)
{
   assert(width_ > 0);
   assert(height_ > 0);
   assert(width_ * height_ <= max_cells);
}

int Board::num_cells(Color color) const noexcept
{
   // If the board has an odd number of cells, the extra cell goes to BLACK.
   return ((width_ * height_) + (1 - color)) / 2;
}

int Board::ordinal(const Cell& cell) const noexcept
{
   return (cell.x + cell.y * width_) / num_colors;
}

Cell Board::cell(Color color, int ordinal) const noexcept
{
   auto full_ordinal = ordinal * 2 + color;
   return { full_ordinal % width_, full_ordinal / width_ };
}

bool Board::out_of_bounds(const Cell& cell) const noexcept
{
   return (cell.x < 0) || (cell.x >= width_) ||
          (cell.y < 0) || (cell.y >= height_);
}

Cells Board::erase_out_of_bounds(const Cells& cells) const
{
   Cells result(cells);
   std::erase_if(result, [this](auto cell) {
      return out_of_bounds(cell);
   });
   return result;
}

BitBoard Board::bitboard(const Cells& cells) const noexcept
{
   auto result = 0;
   for (auto cell : cells) {
      result |= (1 << ordinal(cell));
   }
   return result;
}

Cells Board::cells(Color color, BitBoard bits) const
{
   Cells result;
   for (auto i = 0; i < num_cells(color); ++i) {
      if (bits & (1 << i)) {
         result.push_back(cell(color, i));
      }
   }
   return result;
}

Cell Board::reflect_x(const Cell& cell) const noexcept
{
   return { width_ - 1 - cell.x, cell.y };
}

Cell Board::reflect_y(const Cell& cell) const noexcept
{
   return { cell.x, height_ - 1 - cell.y };
}

Cells Board::reflect_x(const Cells& cells) const
{
   Cells result;
   for (auto cell : cells) {
      result.push_back(reflect_x(cell));
   }
   return result;
}

Cells Board::reflect_y(const Cells& cells) const
{
   Cells result;
   for (auto cell : cells) {
      result.push_back(reflect_y(cell));
   }
   return result;
}

BitBoard Board::reflect_x(Color color, BitBoard bits) const
{
   return bitboard(reflect_x(cells(color, bits)));
}

BitBoard Board::reflect_y(Color color, BitBoard bits) const
{
   return bitboard(reflect_y(cells(color, bits)));
}

BitBoards Board::moves(const Cells& from) const
{
   BitBoards result;
   for (auto i = 0; i < from.size(); ++i) {
      auto to(from);
      // Get from's neighbors and remove any that are off the board.
      auto neighbors = erase_out_of_bounds(from[i].neighbors());
      // Now see if any of the remaining neighbors are blocked by other pieces.
      for (auto neighbor : neighbors) {
         if (std::find(from.begin(), from.end(), neighbor) == from.end()) {
            to[i] = neighbor;
            result.push_back(bitboard(to));
          }
      }
   }
   return result;
}

int num_pieces(BitBoard bits) noexcept
{
   auto num = 0;
   while (bits) {
      if (bits & 1) {
         ++num;
      }
      bits >>= 1;
   }
   return num;
}

std::vector<std::vector<int>> generate_combos(int n, int k)
{
   assert(n > 0);
   assert(k > 0);
   assert(k <= n);

   std::vector<bool> v(n);
   std::fill(v.begin(), v.begin() + k, true);

   std::vector<std::vector<int>> result;
   do {
      std::vector<int> combo;
      for (auto i = 0; i < n; ++i) {
         if (v[i]) {
            combo.push_back(i);
         }
      }
      result.push_back(combo);
   } while (std::prev_permutation(v.begin(), v.end()));

   return result;
}

