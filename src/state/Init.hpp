/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#pragma once

#include <asw/asw.h>

#include "./Game.hpp"
#include "./Menu.hpp"
#include "./State.hpp"

class Init : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float _deltaTime) override {
    sceneManager.registerScene<Menu>(States::Menu, sceneManager);
    sceneManager.registerScene<Game>(States::Game, sceneManager);

    // Goto menu
    sceneManager.setNextScene(States::Menu);
  }
};
