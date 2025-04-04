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
};

class Particle : public asw::game::GameObject {
 public:
  Particle(asw::scene::Scene<States>* scene,
           const asw::Vec2<float>& position,
           asw::Color color,
           float xVelocityMin,
           float xMax,
           float yMin,
           float yMax,
           int size,
           ParticleType type,
           int life,
           ParticleBehaviour behavior);

  void update(float deltaTime) override;

  void draw() override;

 private:
  void drawRandom() const;

  asw::scene::Scene<States>* scene;

  asw::Color color;

  asw::Vec2<float> velocity;

  ParticleType type;
  int life;
  ParticleBehaviour behaviour;
};
