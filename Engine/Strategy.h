//
// Copyright 2023 Stephen E. Bensley
//
// This file is licensed under the MIT License. You may obtain a copy of the
// license at https://github.com/stephenbensley/obatgonu/blob/main/LICENSE.
//

#ifndef Strategy_h
#define Strategy_h

#include "Graph.h"

// Implements an optimal strategy for the game.
class Strategy
{
public:
   Strategy(const Graph& graph);

   // The maximum supported depth. If the game has a deeper graph, this class
   // will have to be updated.
   int max_depth() const noexcept;

   // Execute the strategy and return the best move for the current position.
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

   Entry& find(const Node& node) noexcept;

private:
   Entry find(const Node& node) const noexcept;

   const Graph& graph_;
   std::vector<Entry> entries_;
};

inline int Strategy::max_depth() const noexcept
{
   return 127;
}

inline Strategy::Entry::Entry(int winner, int depth) noexcept
: value_(winner ? -(depth + 1) : (depth + 1))
{ }

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
