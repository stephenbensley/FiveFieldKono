//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include "catch.hpp"
#include "BitOps.h"

TEST_CASE("concat")
{
   CHECK(concat(0x1234, 0x5678) == 0x12345678);
}

TEST_CASE("count_set_bits")
{
   CHECK(count_set_bits(0b0110'1000'1101'0010) == 7);
}

TEST_CASE("interleave")
{
   CHECK(interleave(0xF0F0, 0x0000) == 0x55005500);
   CHECK(interleave(0x0000, 0x0F0F) == 0x00AA00AA);
}

TEST_CASE("even_bits/odd_bits")
{
   uint16_t even = 0x3242;
   uint16_t odd  = 0xfa07;
   uint32_t interleaved = interleave(even, odd);
   CHECK(even_bits(interleaved) == even);
   CHECK(odd_bits(interleaved) == odd);
}
