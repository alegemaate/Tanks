#include "Init.hpp"

#include "../system/ImageRegistry.hpp"
#include "../system/SampleRegistry.hpp"

#include <iostream>

void Init::init() {
  std::cout << "Setting up" << std::endl;

  // Window Title
  asw::display::setTitle("Tanks!");

  // Load images
  ImageRegistry::loadImage("menu-background", "assets/images/menu.png");
  ImageRegistry::loadImage("game-background", "assets/images/background.png");
  ImageRegistry::loadImage("cursor", "assets/images/cursor.png");
  ImageRegistry::loadImage("block-box", "assets/images/block_box_1.png");
  ImageRegistry::loadImage("block-stone", "assets/images/block_stone_1.png");
  ImageRegistry::loadImage("power-up-health",
                           "assets/images/powerup_health.png");
  ImageRegistry::loadImage("power-up-tank-speed",
                           "assets/images/powerup_tank_speed.png");
  ImageRegistry::loadImage("power-up-bullet-speed",
                           "assets/images/powerup_bullet_speed.png");
  ImageRegistry::loadImage("power-up-bullet-delay",
                           "assets/images/powerup_bullet_delay.png");
  ImageRegistry::loadImage("tank-treads", "assets/images/tank_treads.png");
  ImageRegistry::loadImage("tank-dead", "assets/images/tank_dead.png");
  ImageRegistry::loadImage("tank-turret-green",
                           "assets/images/tank_turret_green.png");
  ImageRegistry::loadImage("tank-base-green",
                           "assets/images/tank_base_green.png");
  ImageRegistry::loadImage("tank-turret-red",
                           "assets/images/tank_turret_red.png");
  ImageRegistry::loadImage("tank-base-red", "assets/images/tank_base_red.png");
  ImageRegistry::loadImage("tank-turret-blue",
                           "assets/images/tank_turret_blue.png");
  ImageRegistry::loadImage("tank-base-blue",
                           "assets/images/tank_base_blue.png");
  ImageRegistry::loadImage("light", "assets/images/test.png");

  SampleRegistry::loadSample("explode", "assets/sfx/explode.wav");
  SampleRegistry::loadSample("fire", "assets/sfx/fire.wav");
  SampleRegistry::loadSample("tank-explode", "assets/sfx/tank_explode.wav");

  std::cout << "Loaded images." << std::endl;
}
