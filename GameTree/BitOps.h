//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef BitOps_h
#define BitOps_h

#include <cstdint>

uint32_t concat(uint16_t upper, uint16_t lower) noexcept;
int count_set_bits(uint32_t src) noexcept;
uint32_t interleave(uint16_t even, uint16_t odd) noexcept;
uint16_t even_bits(uint32_t src) noexcept;
uint16_t odd_bits(uint32_t src) noexcept;

#endif /* BitOps_h */
