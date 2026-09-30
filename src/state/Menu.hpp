/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#pragma once

#include <asw/asw.h>
#include <functional>

#include "Game.hpp"
#include "State.hpp"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  asw::ui::Button& addButton(float x, float y, const std::string& text);
  void addStepper(float x, const std::function<void(int)>& change);

  asw::ui::Root ui;
  asw::Font font;
};
