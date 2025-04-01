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

class Particle {
 public:
  Particle(float x,
           float y,
           asw::Color color,
           float xVelocityMin,
           float xMax,
           float yMin,
           float yMax,
           int size,
           ParticleType type,
           int life,
           ParticleBehaviour behavior);

  void update(float deltaTime);

  void draw() const;

  bool getDead() const;

 private:
  void drawRandom() const;

  asw::Quad<float> transform;
  asw::Vec2<float> velocity;

  asw::Color color;

  ParticleType type;
  int life;
  ParticleBehaviour behaviour;

  bool dead = false;
};

#endif  // SRC_GAME_PARTICLE_H_
