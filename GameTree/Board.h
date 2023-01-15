//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef Board_h
#define Board_h

#include <vector>

// This code is only used for graph generation; it is not in the hot path
// during search. Therefore, the code has been optimized for readability and
// simplicity -- not performance.

// The board is a rectangular checkerboard of black and white cells. The
// lower left cell is always black.
enum Color
{
   BLACK,
   WHITE
};
constexpr int num_colors = 2;

// We use bit fields to store the location of the pieces. BitBoard represents
// represents all the pieces of one player.
using BitBoard = uint32_t;
// Since we're using a uint32_t for BitBoard, the board can't have more than
// 32 cells.
constexpr int max_cells = 32;

// Represents pieces of a single color (white or black).
using ColorBitBoard = uint16_t;
using ColorBitBoards = std::vector<ColorBitBoard>;

bool contains(const ColorBitBoards& boards, ColorBitBoard board) noexcept;

class Cell;
using Cells = std::vector<Cell>;

// Represents a position on the game board.
struct Cell
{
   // (x, y) coordinates. (0,0) is the lower left cell of the game board.
   int x;
   int y;

   Cell(int x_, int y_) noexcept;
   Color color() const noexcept;
   
   // All diagonally adjacent cells -- even if they're off the board.
   Cells neighbors() const;
};

bool operator<(const Cell& lhs, const Cell& rhs) noexcept;
bool operator==(const Cell& lhs, const Cell& rhs) noexcept;
bool operator==(const Cells& lhs, const Cells& rhs) noexcept;
bool contains(const Cells& cells, const Cell& cell) noexcept;

// The minimum number of moves to go from one cell to another.
int distance(const Cell& lhs, const Cell& rhs) noexcept;

// The minimum number of moves to go from one board position to another.
int distance(const Cells& lhs, const Cells& rhs) noexcept;

// Represents the game board.
class Board
{
public:
   Board(int width, int height) noexcept;

   int num_cells() const noexcept;
   int num_cells(Color color) const noexcept;

   // Ordinals are zero-indexed starting at the lower left and increasing first
   // horizontally and then vertically.
   int ordinal(const Cell& cell) const noexcept;
   Cell cell(int ordinal) const noexcept;

   // Per-color ordinals are incremented separately for each color in order to
   // keep the ColorBitBoards densley packed. For example, a 3x3 board looks
   // like this:
   //    3 3 4
   //    1 2 2
   //    0 0 1
   int color_ordinal(const Cell& cell) const noexcept;
   Cell cell(Color color, int ordinal) const noexcept;

   bool out_of_bounds(const Cell& cell) const noexcept;
   Cells erase_out_of_bounds(const Cells& cells) const;

   BitBoard bitboard(const Cells& cells) const noexcept;
   Cells cells(BitBoard bits) const;
   ColorBitBoard color_bitboard(const Cells& cells) const noexcept;
   Cells cells(Color color, ColorBitBoard bits) const;

   Cell reflect_x(const Cell& cell) const noexcept;
   Cell reflect_y(const Cell& cell) const noexcept;

   Cells reflect_x(const Cells& cells) const;
   Cells reflect_y(const Cells& cells) const;

   ColorBitBoard reflect_x(Color color, ColorBitBoard bits) const;
   ColorBitBoard reflect_y(Color color, ColorBitBoard bits) const;

   // Returns all legal board positions that can be reached from the given
   // cells in a single move.
   ColorBitBoards moves(const Cells& from) const;

private:
   int width_;
   int height_;
};

// Helper function to generate all possible combinations C(n, k). Useful for
// enumerating all board positions.
std::vector<std::vector<int>> generate_combos(int n, int k);

inline Cell::Cell(int x_, int y_) noexcept
: x(x_), y(y_)
{ }

inline Color Cell::color() const noexcept
{
   // For black squares, the row and column parity are the same.
   return  ((x % 2) == (y % 2)) ? BLACK : WHITE;
}

#endif /* Board_h */
