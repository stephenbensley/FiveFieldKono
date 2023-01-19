//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef Strategy_h
#define Strategy_h

#include "Graph.h"
#include <limits>

// Implements an optimal strategy for the game.
class Strategy
{
public:
   Strategy(const Graph& graph);

   // The maximum supported depth. If the game has a deeper graph, this class
   // will have to be updated.
   static constexpr int max_depth() noexcept;

   // Returns the best move for the current position.
   Node best_move(const Node& from) const noexcept;

   // Load/save the strategy from/to a file.
   bool load(const char* filename);
   void save(const char* filename) const noexcept;

   // Entry for a node in the strategy table.
   class Entry
   {
   public:
      Entry() = default;
      Entry(int winner, int depth) noexcept;
      bool empty() const noexcept;
      int winner() const noexcept;
      int value() const noexcept;

   private:
      char value_;
   };

   // Only used when building a new strategy.
   Entry& find(const Node& node) noexcept;

private:
   Entry find(const Node& node) const noexcept;

   const Graph& graph_;
   std::vector<Entry> entries_;
};

constexpr int Strategy::max_depth() noexcept
{
   // Anything bigger than this will cause an overflow in the constructor.
   return std::numeric_limits<char>::max() - 1;
}

inline Strategy::Entry::Entry(int winner, int depth) noexcept
: value_(winner ? -(depth + 1) : (depth + 1))
{
   assert(depth <= max_depth());
}

inline bool Strategy::Entry::empty() const noexcept
{
   return value_ == 0;
}

inline int Strategy::Entry::winner() const noexcept
{
   return (value_ > 0) ? 0 : 1;
}

inline int Strategy::Entry::value() const noexcept
{
   return value_;
}

#endif /* Strategy_h */
