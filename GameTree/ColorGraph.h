#ifndef ColorGraph_h
#define ColorGraph_h

#include "Board.h"
#include <unordered_map>

constexpr int num_players = 2;

bool is_valid_player(int player) noexcept;
int other_player(int player) noexcept;

class ColorNode;
using ColorNodes = std::vector<const ColorNode*>;

struct ColorNode {
   struct Player {
      BitBoard pieces;
      bool goal_reached;
      bool goal_full;
      short distance;
      ColorNodes moves;
   };

   uint16_t index;
   Player player[num_players];
};

class ColorGraph
{
public:
   ColorGraph(const Board& board,
              Color color,
              BitBoard start0);

   const ColorNode* root() const noexcept;
   int size() const noexcept;

private:
   struct Position {
      BitBoard pieces;
      BitBoard reflected;
      short distance[num_players];
      BitBoards moves;
   };
   using Positions = std::vector<Position>;

   static Positions build_positions(const Board& board,
                                    Color color,
                                    const Cells& goal0,
                                    const Cells& goal1);
   static std::pair<uint32_t, uint32_t> get_keys(const Position& p0,
                                                 const Position& p1) noexcept;
   static bool is_valid_combo(const Position& p0,
                              const Position& p1) noexcept;

   static ColorNode build_node(uint16_t index,
                               const Position& p0,
                               const Position& p1,
                               BitBoard goal0,
                               BitBoard goal1);

   void build_nodes(const Positions& positions,
                    BitBoard goal0,
                    BitBoard goal1);

   ColorNode* find(BitBoard p0, BitBoard p1) noexcept;

   ColorNodes build_p0_moves(const Position& p0,
                             const Position& p1);
   ColorNodes build_p1_moves(const Position& p0,
                             const Position& p1);
   void build_moves(ColorNode& dst,
                    const Position& p0,
                    const Position& p1);
   void populate_moves(const Positions& positions);



   std::vector<ColorNode> nodes_;
   std::unordered_map<uint32_t, int> index_;
   const ColorNode* root_ = nullptr;
};

inline const ColorNode* ColorGraph::root() const noexcept
{
   assert(root_ != nullptr);
   return root_;
}

inline int ColorGraph::size() const noexcept
{
   return static_cast<int>(nodes_.size());
}

inline bool is_valid_player(int player) noexcept
{
   return (player >= 0) && (player < num_players);
}

inline int other_player(int player) noexcept
{
   return player ? 0 : 1;
}

#endif /* ColorGraph_h */
