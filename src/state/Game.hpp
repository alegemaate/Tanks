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
  void generateMap();

  // Images
  asw::Texture map_buffer;
  asw::Texture decal_buffer;
  asw::Texture light_buffer;
  asw::Texture fade_buffer;
  asw::Texture background;
  asw::Texture cursor;

  // Fonts
  asw::Font font;

  asw::Vec2<float> map_position;

  int currentRound = 0;

  float timer = 0.0f;
};
