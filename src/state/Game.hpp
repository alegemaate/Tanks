/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
 **/
#pragma once

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

class Game : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;

  void draw() override;

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
  std::vector<std::shared_ptr<Tank>> enemy_tanks;
  std::vector<std::shared_ptr<Tank>> player_tanks;
  std::vector<PowerUp> power_ups;

  float map_x = 0;
  float map_y = 0;

  int currentRound = 0;
};
