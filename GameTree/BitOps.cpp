//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#include <cstdint>

uint32_t concat(uint16_t upper, uint16_t lower) noexcept
{
   return static_cast<uint32_t>(upper) << 16 |
          static_cast<uint32_t>(lower);
}

int count_set_bits(uint32_t src) noexcept
{
   // From: https://graphics.stanford.edu/~seander/bithacks.html

   src = src - ((src >> 1) & 0x55555555);
   src = (src & 0x33333333) + ((src >> 2) & 0x33333333);
   return ((src + (src >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

uint32_t interleave(uint16_t even, uint16_t odd) noexcept
{
   // From: https://graphics.stanford.edu/~seander/bithacks.html

   static const unsigned int B[] = {
      0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF
   };
   static const unsigned int S[] = { 1, 2, 4, 8 };

   uint32_t v = even;
   v = (v | (v << S[3])) & B[3];
   v = (v | (v << S[2])) & B[2];
   v = (v | (v << S[1])) & B[1];
   v = (v | (v << S[0])) & B[0];

   uint32_t d = odd;
   d = (d | (d << S[3])) & B[3];
   d = (d | (d << S[2])) & B[2];
   d = (d | (d << S[1])) & B[1];
   d = (d | (d << S[0])) & B[0];

   return v | (d << 1);

}

uint16_t even_bits(uint32_t src) noexcept
{
   // From: https://stackoverflow.com/questions/4909263/how-to-efficiently-de-interleave-bits-inverse-morton
   
   src = src & 0x55555555;
   src = (src | (src >> 1)) & 0x33333333;
   src = (src | (src >> 2)) & 0x0F0F0F0F;
   src = (src | (src >> 4)) & 0x00FF00FF;
   src = (src | (src >> 8)) & 0x0000FFFF;
   return src;
}

uint16_t odd_bits(uint32_t src) noexcept
{
   return even_bits(src >> 1);
}

