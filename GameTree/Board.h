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

// We use bit fields to store the location of the pieces. There is one field for
// each (player, color) pair.
using BitBoard = uint16_t;
using BitBoards = std::vector<BitBoard>;

bool contains(const BitBoards& boards, BitBoard board) noexcept;

// Since we're using a 16 bits for the BitBoard, each color can have at most
// 16 cells, so the board can have at most 32.
constexpr int max_cells = 32;

class Cell;
using Cells = std::vector<Cell>;

// Represents a position on the game board.
struct Cell
{
   // (x, y) coordinates. (0,0) is the lower left cell of the game board.
   int x;
   int y;

   Cell(int x_, int y_) noexcept;

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

   int num_cells(Color color) const noexcept;

   // Ordinals are zero-indexed starting at the lower left and increasing first
   // horizontally and then vertically. Ordinals are incremented separately
   // for each color in order to keep the BitBoards densley packed. For example,
   // a 3x3 board looks like this:
   //    3 3 4
   //    1 2 2
   //    0 0 1
   int ordinal(const Cell& cell) const noexcept;
   Cell cell(Color color, int ordinal) const noexcept;

   bool out_of_bounds(const Cell& cell) const noexcept;
   Cells erase_out_of_bounds(const Cells& cells) const;

   BitBoard bitboard(const Cells& cells) const noexcept;
   Cells cells(Color color, BitBoard bits) const;

   Cell reflect_x(const Cell& cell) const noexcept;
   Cell reflect_y(const Cell& cell) const noexcept;

   Cells reflect_x(const Cells& cells) const;
   Cells reflect_y(const Cells& cells) const;

   BitBoard reflect_x(Color color, BitBoard bits) const;
   BitBoard reflect_y(Color color, BitBoard bits) const;

   // Returns all legal board positions that can be reached from the given
   // cells in a single move.
   BitBoards moves(const Cells& from) const;

private:
   int width_;
   int height_;
};

// Number of pieces represented by the BitBoard.
int num_pieces(BitBoard bits) noexcept;

// Helper function to generate all possible combinations C(n, k). Useful for
// enumerating all board positions.
std::vector<std::vector<int>> generate_combos(int n, int k);

inline Cell::Cell(int x_, int y_) noexcept
: x(x_), y(y_)
{ }

#endif /* Board_h */
