#ifndef GameDefs_h
#define GameDefs_h

#include <cassert>
#include <cstdint>

constexpr int num_players = 2;

inline bool is_valid_player(int player) noexcept
{
   return (player >= 0) && (player < num_players);
}

inline int other_player(int player) noexcept
{
   return player ? 0 : 1;
}

using BitBoard = uint16_t;
constexpr int max_cells = 32;

enum Color
{
   BLACK,
   WHITE
};
constexpr int num_colors = 2;

#endif /* GameDefs_h */
