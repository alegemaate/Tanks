#include "TileMap.hpp"

// Generate a new map
void TileMap::generateMap(unsigned char mapWidth, unsigned char mapHeight) {
  // Assign new mapWidth and mapHeight
  width = mapWidth;
  height = mapHeight;

  // Setup map
  for (unsigned char i = 0; i < width; i++) {
    std::vector<int> row(width, 0);
    tile_map.push_back(row);
  }

  const unsigned char passes = 7;

  // Make a map
  for (unsigned char pass = 0; pass < passes; pass++) {
    for (unsigned char i = 0; i < width; i++) {
      for (unsigned char t = 0; t < height; t++) {
        auto neighbours = this->getNeighbours(i, t);

        switch (pass) {
          // Pass 1 (Edges)
          case 1:
            if (neighbours.n == -1 || neighbours.e == -1 ||
                neighbours.s == -1 || neighbours.w == -1) {
              tile_map[i][t] = 1;
            }
            break;

          // Pass 2 (Well Placed blocks)
          case 2: {
            const bool should_place = asw::random::between(0, 2) == 1;

            if (should_place && neighbours.n == 0 && neighbours.e == 0 &&
                neighbours.s == 0 && neighbours.w == 0 && neighbours.ne == 0 &&
                neighbours.se == 0 && neighbours.sw == 0 &&
                neighbours.nw == 0) {
              tile_map[i][t] = 1;
            }
            break;
          }

          // Pass 3 (Filling)
          case 3: {
            if ((neighbours.e == 1 && neighbours.w == 1) ||
                (neighbours.n == 1 && neighbours.s == 1)) {
              tile_map[i][t] = 1;
            }
            break;
          }

          // Pass 4 (Filling inaccessible areas)
          case 4: {
            if (neighbours.e == 1 && neighbours.w == 1 && neighbours.n == 1 &&
                neighbours.s == 1) {
              tile_map[i][t] = 1;
            }
            break;
          }

          // Pass 5 (Boxes!)
          case 5: {
            if (tile_map[i][t] == 0 && asw::random::between(1, 20) == 1) {
              tile_map[i][t] = 2;
            }
            break;
          }

          // Pass 6 (Find start locations)
          case 6: {
            if (tile_map[i][t] == 0) {
              start_locations.emplace_back(i * 40, t * 40);
            }
            break;
          }

          default:
            break;
        }
      }
    }
  }
}

TileMapNeighbours TileMap::getNeighbours(const unsigned char x,
                                         const unsigned char y) const {
  TileMapNeighbours neighbours{};

  const bool n_safe = y > 0;
  const bool s_safe = y < height - 1;
  const bool e_safe = x < width - 1;
  const bool w_safe = x > 0;

  neighbours.n = n_safe ? tile_map[x][y - 1] : -1;
  neighbours.e = e_safe ? tile_map[x + 1][y] : -1;
  neighbours.s = s_safe ? tile_map[x][y + 1] : -1;
  neighbours.w = w_safe ? tile_map[x - 1][y] : -1;

  neighbours.ne = n_safe && e_safe ? tile_map[x + 1][y - 1] : -1;
  neighbours.se = s_safe && e_safe ? tile_map[x + 1][y + 1] : -1;
  neighbours.sw = s_safe && w_safe ? tile_map[x - 1][y + 1] : -1;
  neighbours.nw = n_safe && w_safe ? tile_map[x - 1][y - 1] : -1;

  return neighbours;
}
