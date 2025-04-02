#ifndef SRC_GAME_PARTICLE_H_
#define SRC_GAME_PARTICLE_H_

#include <asw/asw.h>
#include "../util/tools.h"

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
  Particle(const asw::Vec2<float>& position,
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

  asw::Color color;

  asw::Vec2<float> velocity;

  ParticleType type;
  int life;
  ParticleBehaviour behaviour;
};

#endif  // SRC_GAME_PARTICLE_H_
