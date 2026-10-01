#pragma once

#include <vector>

#include "../state/State.hpp"
#include "./Particle.hpp"

enum class BounceDirection {
  NONE,
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
};

class Bullet : public asw::game::GameObject {
 public:
  Bullet(asw::scene::Scene<States>* scene,
         float x,
         float y,
         float angle,
         float speed,
         int health,
         int team);

  void update(float dt) override;
  void draw() override;
  void destroy();

  int getTeam() const { return team; }

  void drawLight() {
    asw::draw::stretch_sprite(light_buffer,
                              transform + asw::Quad<float>(-4, -4, 8, 8));
  }

 private:
  bool hitBarriers();

  asw::scene::Scene<States>* scene;

  asw::Texture light_buffer;

  int team;
  int health;
  BounceDirection incidenceDirection{BounceDirection::NONE};
};
