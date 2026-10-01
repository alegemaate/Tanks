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

  void update(float dt) override;

  void draw() override;

  // Map stuff
  static constexpr unsigned char MIN_MAP_SIZE = 5;
  static unsigned char map_width;
  static unsigned char map_height;

  static unsigned char num_enemies;
  static unsigned char num_friends;

 private:
  enum class RoundState { Playing, Won, Lost };

  void startRound();
  void generateMap();
  void updateRoundState();

  // Images
  asw::Texture map_buffer;
  asw::Texture decal_buffer;
  asw::Texture light_buffer;
  asw::Texture fade_buffer;
  asw::Texture background;
  asw::Texture cursor;

  // Fonts
  asw::Font font;

  asw::Camera camera;

  int currentRound = 0;

  RoundState roundState = RoundState::Playing;

  // Seconds since the round was won or lost
  float timer = 0.0f;

  int friendsLeft = 0;
  int enemiesLeft = 0;
};
