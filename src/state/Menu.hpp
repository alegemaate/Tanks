/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#pragma once

#include "../ui/Button.hpp"
#include "Game.hpp"
#include "State.hpp"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;
  void update(float deltaTime) override;
  void draw() override;

 private:
  std::shared_ptr<Button> enemies_up;
  std::shared_ptr<Button> enemies_down;
  std::shared_ptr<Button> friends_up;
  std::shared_ptr<Button> friends_down;
  std::shared_ptr<Button> width_up;
  std::shared_ptr<Button> width_down;
  std::shared_ptr<Button> height_up;
  std::shared_ptr<Button> height_down;
  std::shared_ptr<Button> bounce_up;
  std::shared_ptr<Button> bounce_down;
  std::shared_ptr<Button> start;

  asw::Font font;
};
