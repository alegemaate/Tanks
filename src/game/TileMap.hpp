#ifndef SRC_GAME_TILE_MAP_H_
#define SRC_GAME_TILE_MAP_H_

#include <string>
#include <vector>

#include "Barrier.hpp"

struct TileMapNeighbours {
  int n;
  int ne;
  int e;
  int se;
  int s;
  int sw;
  int w;
  int nw;
};

class TileMap {
 public:
  void generateMap(unsigned char mapWidth, unsigned char mapHeight);

 private:
  TileMapNeighbours getNeighbours(unsigned char x, unsigned char y) const;

  unsigned char width;
  unsigned char height;

  std::vector<Barrier> barriers;
  std::vector<asw::Vec2<int>> start_locations;

  std::vector<std::vector<int>> tile_map;
};

#endif  // SRC_GAME_TILE_MAP_H_
