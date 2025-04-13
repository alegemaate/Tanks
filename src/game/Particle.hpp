#pragma once

#include <asw/asw.h>

#include "../state/State.hpp"

enum class ParticleType {
  CIRCLE,
  SQUARE,
  PIXEL,
  RANDOM,
};

enum class ParticleBehaviour {
  EXPLODE,
  FIRE,
};

class Particle : public asw::game::GameObject {
 public:
  Particle(asw::scene::Scene<States>* scene,
           const asw::Vec2<float>& position,
           asw::Color color,
           float xVelocityMin,
           float xVelocityMax,
           float yVelocityMin,
           float yVelocityMax,
           int size,
           ParticleType type,
           int life,
           ParticleBehaviour behavior);

  void update(float deltaTime) override;

  void draw() override;

  void drawLight() {
    asw::draw::stretchSprite(light_buffer,
                             transform + asw::Quad<float>(-1, -1, 2, 2));
  }

 private:
  asw::scene::Scene<States>* scene;

  asw::Color color;

  asw::Vec2<float> velocity;

  ParticleType type;
  int life;
  ParticleBehaviour behaviour;

  asw::Texture light_buffer;
};
