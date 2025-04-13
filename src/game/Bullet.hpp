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

  void update(float deltaTime) override;
  void draw() override;
  void destroy();

  float getXVelocity() const { return velocity.x; }

  float getYVelocity() const { return velocity.y; }

  int getTeam() const { return team; }

  void drawLight() {
    asw::draw::stretchSprite(light_buffer,
                             transform + asw::Quad<float>(-4, -4, 8, 8));
  }

 private:
  void bounce(BounceDirection direction);
  void reverseDirection(const std::string& direction);

  asw::scene::Scene<States>* scene;
  asw::Vec2<float> velocity;

  asw::Texture light_buffer;

  int team;
  int health;
  BounceDirection incidenceDirection{BounceDirection::NONE};
};
