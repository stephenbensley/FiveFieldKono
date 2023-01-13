#ifndef Board_h
#define Board_h

#include "GameDefs.h"
#include <vector>

using BitBoards = std::vector<BitBoard>;

class Cell;
using Cells = std::vector<Cell>;

struct Cell
{
   int x;
   int y;

   Cells neighbors() const;
};

bool operator==(const Cell& lhs, const Cell& rhs) noexcept;

int distance(const Cell& lhs, const Cell& rhs) noexcept;
int distance(const Cells& lhs, const Cells& rhs) noexcept;

class Board
{
public:
   Board(int width, int height) noexcept;

   int num_cells(Color color) const noexcept;
   
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

   BitBoards moves(const Cells& from) const;

private:
   int width_;
   int height_;
};

int num_pieces(BitBoard bits) noexcept;

std::vector<std::vector<int>> generate_combos(int n, int k);

#endif /* Board_h */
