//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef ToString_h
#define ToString_h

#include "Board.h"
#include <string>

std::string to_string(const Board& board,
                      const GamePosition& pos);

std::string to_string(const Board& board,
                      const GamePosition& from,
                      const GamePosition& to);

#endif /* ToString_h */
