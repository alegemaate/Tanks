/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#pragma once

#include <asw/asw.h>
#include <entt/entt.hpp>

#include "../systems/RenderSystem.hpp"
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
  Button enemies_up;
  Button enemies_down;
  Button friends_up;
  Button friends_down;
  Button width_up;
  Button width_down;
  Button height_up;
  Button height_down;
  Button bounce_up;
  Button bounce_down;
  Button start;

  asw::Font font;

  entt::registry m_registry;

  entt::dispatcher m_dispatcher;

  RenderSystem m_render_system;
};
