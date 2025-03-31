/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_GAME_H_
#define SRC_STATE_GAME_H_

#include <asw/asw.h>
#include <array>
#include <memory>
#include <vector>

#include "../game/AiTank.hpp"
#include "../game/Barrier.hpp"
#include "../game/PlayerTank.hpp"
#include "../game/PowerUp.hpp"
#include "../game/Tank.hpp"
#include "../game/World.hpp"
#include "../util/tools.h"

#include "./State.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

class Game : public State {
 public:
  explicit Game(StateEngine& engine) : State(engine) {}

  void init() override;
  void update(const float deltaTime) override;
  void draw() override;
  void cleanup() override {
    // No cleanup
  }

  // Map stuff
  static unsigned char map_width;
  static unsigned char map_height;

  static unsigned char num_enemies;
  static unsigned char num_friends;

 private:
  // Images
  asw::Texture map_buffer;
  asw::Texture decal_buffer;
  asw::Texture background;
  asw::Texture cursor;

  // Fonts
  asw::Font font;

  // World
  World game_world;

  // Objects
  std::vector<Barrier> barriers;
  std::vector<Tank*> enemy_tanks;
  std::vector<Tank*> player_tanks;
  std::vector<PowerUp> power_ups;
  std::vector<asw::Vec2<float>> startLocations;

  float map_x = 0;
  float map_y = 0;

  int currentRound = 0;
};

#endif  // SRC_STATE_GAME_H_
