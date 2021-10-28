/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_GAME_H_
#define SRC_STATE_GAME_H_

#include <allegro.h>
#include <time.h>
#include <cmath>
#include <vector>

#include "../game/barrier.h"
#include "../game/powerup.h"
#include "../game/tank.h"
#include "../game/world.h"
#include "../util/coordinate.h"
#include "../util/tools.h"

#include "./state.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

#define number_of_rays 200

class game : public state {
 public:
  game();
  ~game();

  void update();
  void draw();

  // Map stuff
  static const unsigned char max_map_width = 255;
  static const unsigned char max_map_height = 255;

  static unsigned char map_width;
  static unsigned char map_height;

  static unsigned char num_enemies;
  static unsigned char num_friends;

 private:
  // Images
  BITMAP* buffer;
  BITMAP* map_buffer;
  BITMAP* decal_buffer;
  BITMAP* vision_buffer;
  BITMAP* background;
  BITMAP* cursor;
  BITMAP* blocks[3];
  BITMAP* powerup_images[4];
  BITMAP* tank_images[10];

  // World
  world game_world;

  // Objects
  std::vector<Barrier> barriers;
  std::vector<Tank*> enemy_tanks;
  std::vector<Tank*> player_tanks;
  std::vector<Powerup> powerups;
  std::vector<Coordinate> startLocations;

  int map_temp[max_map_width][max_map_height];
  int map_x, map_y;

  int currentRound = 0;
};

#endif  // SRC_STATE_GAME_H_