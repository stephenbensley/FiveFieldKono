//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "ColorGraph.h"

TEST_CASE("ColorGraph::size")
{
   // Black has 5 cells; 2 pieces per player.
   // 5!/(2!*2!) = 30 positions.
   // Only the starting position and its y-reflection are symmetric, so size is
   // (30 - 2)/2 + 2 = 16.
   //    O O
   //     -
   //    X X
   CHECK(ColorGraph({3,3}, BLACK, 0b11).size() == 16);

   // White has four cells and 1 piece per player.
   // 4!/2! = 12 positions.
   // Only the starting position and its y-reflection are symmetric, so size is
   // (12 - 2)/2 + 2 = 7.
   //     O
   //    - -
   //     X
   CHECK(ColorGraph({3,3}, WHITE, 0b01).size() == 7);
}

TEST_CASE("ColorGraph::start")
{
   SECTION("Black initial position")
   {
      // Start position for black:
      //    O O O
      //     - -
      //    - - -
      //     - -
      //    X X X
      //
      ColorGraph graph({5,5}, BLACK, 0b000'00'000'00'111);
      auto start = graph.start();

      CHECK(start->player[0].pieces == 0b000'00'000'00'111);
      CHECK(!start->player[0].goal_reached);
      CHECK(start->player[0].goal_full);
      CHECK(start->player[0].distance == 12);

      CHECK(start->player[1].pieces == 0b111'00'000'00'000);
      CHECK(!start->player[1].goal_reached);
      CHECK(start->player[1].goal_full);
      CHECK(start->player[0].distance == 12);

      // Only two moves for X:
      //    O O O
      //     - -
      //    - - -
      //     X -
      //    - X X
      //
      //    O O O
      //     - -
      //    - - -
      //     X -
      //    X - X
      //
      REQUIRE(start->player[0].moves.size() == 2);
      CHECK(start->player[0].moves[0]->player[0].pieces == 0b000'00'000'01'110);
      CHECK(start->player[0].moves[0]->player[1].pieces == 0b111'00'000'00'000);
      CHECK(start->player[0].moves[1]->player[0].pieces == 0b000'00'000'01'101);
      CHECK(start->player[0].moves[1]->player[1].pieces == 0b111'00'000'00'000);

      // O's moves:
      //    O O -
      //     - O
      //    - - -
      //     X -
      //    X X X
      //
      //    O - O
      //     O -
      //    - - -
      //     - -
      //    X X X
      //
      REQUIRE(start->player[0].moves.size() == 2);
      CHECK(start->player[1].moves[0]->player[1].pieces == 0b011'10'000'00'000);
      CHECK(start->player[1].moves[0]->player[0].pieces == 0b000'00'000'00'111);
      CHECK(start->player[1].moves[1]->player[1].pieces == 0b101'01'000'00'000);
      CHECK(start->player[1].moves[1]->player[0].pieces == 0b000'00'000'00'111);
   }

   SECTION("White initial position")
   {
      // Start position for white:
      //     O O
      //    O - O
      //     - -
      //    X - X
      //     X X
      //
      ColorGraph graph({5,5}, WHITE, 0b00'000'00'101'11);
      auto start = graph.start();

      CHECK(start->player[0].pieces == 0b00'000'00'101'11);
      CHECK(!start->player[0].goal_reached);
      CHECK(start->player[0].goal_full);
      CHECK(start->player[0].distance == 12);

      CHECK(start->player[1].pieces == 0b11'101'00'000'00);
      CHECK(!start->player[1].goal_reached);
      CHECK(start->player[1].goal_full);
      CHECK(start->player[0].distance == 12);

      // Only two moves for X:
      //     O O
      //    O - O
      //     - -
      //    X X X
      //     X -
      //
      //     O O
      //    O - O
      //     X -
      //    - - X
      //     X X
      //
      REQUIRE(start->player[0].moves.size() == 2);
      CHECK(start->player[0].moves[0]->player[0].pieces == 0b00'000'00'111'01);
      CHECK(start->player[0].moves[0]->player[1].pieces == 0b11'101'00'000'00);
      CHECK(start->player[0].moves[1]->player[0].pieces == 0b00'000'01'100'11);
      CHECK(start->player[0].moves[1]->player[1].pieces == 0b11'101'00'000'00);

      // O's moves:
      //     O O
      //    O - -
      //     - O
      //    X - X
      //     X X
      //
      //     O -
      //    O O O
      //     - -
      //    X - X
      //     X X
      //
      REQUIRE(start->player[0].moves.size() == 2);
      CHECK(start->player[1].moves[0]->player[1].pieces == 0b11'001'10'000'00);
      CHECK(start->player[1].moves[0]->player[0].pieces == 0b00'000'00'101'11);
      CHECK(start->player[1].moves[1]->player[1].pieces == 0b01'111'00'000'00);
      CHECK(start->player[1].moves[1]->player[0].pieces == 0b00'000'00'101'11);
   }
}
