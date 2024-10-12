//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/FiveFieldKono/blob/main/LICENSE.
//

#include "catch.hpp"
#include "Board.h"

TEST_CASE("Cell::color")
{
   CHECK(Cell(0,0).color() == BLACK);
   CHECK(Cell(1,0).color() == WHITE);
   CHECK(Cell(0,1).color() == WHITE);
   CHECK(Cell(1,1).color() == BLACK);
}

TEST_CASE("Cell:neighbors")
{
   auto neighbors = Cell(2,2).neighbors();
   REQUIRE(neighbors.size() == 4);
   CHECK(neighbors[0] == Cell(1,1));
   CHECK(neighbors[1] == Cell(1,3));
   CHECK(neighbors[2] == Cell(3,1));
   CHECK(neighbors[3] == Cell(3,3));
}

TEST_CASE("Cell::operator==")
{
   CHECK(Cell(3,3) == Cell(3,3));
   CHECK(Cell(3,3) != Cell(2,3));
   CHECK(Cell(3,3) != Cell(3,2));
   CHECK(Cell(3,3) != Cell(2,2));
}

TEST_CASE("Cell::operator<")
{
   CHECK(  Cell(1,2) < Cell(3,4));
   CHECK(  Cell(1,2) < Cell(3,2));
   CHECK(!(Cell(1,2) < Cell(3,1)));
   CHECK(  Cell(1,2) < Cell(1,4));
   CHECK(!(Cell(1,2) < Cell(1,2)));
   CHECK(!(Cell(1,2) < Cell(1,1)));
   CHECK(  Cell(1,2) < Cell(0,4));
   CHECK(!(Cell(1,2) < Cell(0,2)));
   CHECK(!(Cell(1,2) < Cell(0,1)));
}

TEST_CASE("distance(Cell)")
{
   // Reflexive
   CHECK(distance({1,1}, {1,1}) == 0);
   // Symmetric
   CHECK(distance({1,2}, {3,4}) == 2);
   CHECK(distance({3,4}, {1,2}) == 2);
   // Move in x, y, or both yields same result.
   CHECK(distance({1,2}, {1,4}) == 2);
   CHECK(distance({1,2}, {3,2}) == 2);
}

TEST_CASE("distance(Cells")
{
   Cells lhs = {{0,0}, {1,2}, {4,3}};
   Cells rhs = {{1,1}, {4,2}, {0,3}};

   // 0,0 - 1,1 = 1
   // 1,2 - 0,3 = 1
   // 4,3 - 4,2 = 1
   CHECK(distance(lhs, rhs) == 3);
   // Reflexive
   CHECK(distance(lhs, lhs) == 0);
   // Symmetric
   CHECK(distance(rhs, lhs) == 3);

   // Permuting cells doesn't change distance
   while (std::next_permutation(lhs.begin(), lhs.end())) {
      CHECK(distance(lhs, rhs) == 3);
   }
}

TEST_CASE("Board::num_cells")
{
   CHECK(Board(3,4).num_cells() == 12);

   // Even number of cells.
   CHECK(Board(3,4).num_cells(BLACK) == 6);
   CHECK(Board(3,4).num_cells(WHITE) == 6);

   // Odd number of cells.
   CHECK(Board(5,5).num_cells(BLACK) == 13);
   CHECK(Board(5,5).num_cells(WHITE) == 12);
}

TEST_CASE("Board::ordinal <--> Board::cell")
{
   SECTION("Odd board width")
   {
      Board board(3, 3);
      for (auto x = 0; x < 3; ++x) {
         for (auto y = 0; y < 3; ++y) {
            Cell c(x, y);
            CHECK(board.cell(board.ordinal(c)) == c);
            CHECK(board.cell(c.color(), board.color_ordinal(c)) == c);
         }
      }
   }

   SECTION("Even board width")
   {
      Board board(4, 4);
      for (auto x = 0; x < 4; ++x) {
         for (auto y = 0; y < 4; ++y) {
            Cell c(x, y);
            CHECK(board.cell(board.ordinal(c)) == c);
            CHECK(board.cell(c.color(), board.color_ordinal(c)) == c);
         }
      }
   }
}

TEST_CASE("Board::out_of_bounds")
{
   Board board(5, 5);
   CHECK(!board.out_of_bounds({ 0,4}));
   CHECK( board.out_of_bounds({-1,4}));
   CHECK( board.out_of_bounds({ 0,5}));
   CHECK( board.out_of_bounds({-1,5}));
}

TEST_CASE("Board::erase_out_of_bounds")
{
   Board board(5, 5);
   Cells all_in = {{1,2}, {4,0}, {3,0}};
   CHECK(board.erase_out_of_bounds(all_in) == all_in);

   Cells one_out = {{1,2}, {4,-1}, {3,0}};
   auto erased = board.erase_out_of_bounds(one_out);
   REQUIRE(erased.size() == 2);
   CHECK(erased[0] == Cell(1,2));
   CHECK(erased[1] == Cell(3,0));
}

TEST_CASE("Board::bitboard <--> Board::cells")
{
   Board board(5, 5);

   Cells black = {{1,3}, {3,1}, {3,3}};
   auto all_bits = board.bitboard(black);
   auto color_bits = board.color_bitboard(black);
   CHECK(black == board.cells(all_bits));
   CHECK(black == board.cells(BLACK, color_bits));
   CHECK(color_bits == board.color_bitboard(BLACK, all_bits));

   Cells white = {{1,2}, {4,1}, {3,0}};
   all_bits = board.bitboard(white);
   color_bits = board.color_bitboard(white);
   CHECK(white == board.cells(all_bits));
   CHECK(white == board.cells(WHITE, color_bits));
   CHECK(color_bits == board.color_bitboard(WHITE, all_bits));

   Cells both(black);
   both.insert(both.end(), white.begin(), white.end());
   CHECK(both == board.cells(board.bitboard(both)));
}

TEST_CASE("Board::reflect_x")
{
   Board board(5, 5);
   CHECK(board.reflect_x(Cell(0,4)) == Cell(4,4));
   CHECK(board.reflect_x(Cell(2,4)) == Cell(2,4));
}

TEST_CASE("Board::reflect_y")
{
   Board board(5, 5);
   CHECK(board.reflect_y(Cell(4,0)) == Cell(4,4));
   CHECK(board.reflect_y(Cell(4,2)) == Cell(4,2));
}

TEST_CASE("Board::moves")
{
   Board board(5,5);

   SECTION("Black initial position") {
      Cells from = {{0,0}, {0,2}, {0,4}};
      Cells expected[] = {
         {{1,1}, {0,2}, {0,4}},
         {{0,0}, {1,1}, {0,4}},
         {{0,0}, {1,3}, {0,4}},
         {{0,0}, {0,2}, {1,3}}
      };
      auto to = board.moves(from);
      CHECK(to.size() == sizeof(expected)/sizeof(Cells));
      for (auto cells : expected) {
         CHECK(contains(to, board.color_bitboard(cells)));
      }
   }

   SECTION("White initial position") {
      Cells from = {{1,0}, {3,0}, {0,1}, {4,1}};
      Cells expected[] = {
         {{2,1}, {3,0}, {0,1}, {4,1}},
         {{1,0}, {2,1}, {0,1}, {4,1}},
         {{1,0}, {3,0}, {1,2}, {4,1}},
         {{1,0}, {3,0}, {0,1}, {3,2}}
      };
      auto to = board.moves(from);
      CHECK(to.size() == sizeof(expected)/sizeof(Cells));
      for (auto cells : expected) {
         CHECK(contains(to, board.color_bitboard(cells)));
      }
   }
}
