#ifndef SRC_GAME_BULLET_H_
#define SRC_GAME_BULLET_H_

#include <vector>

#include "../util/tools.h"
#include "./Particle.hpp"
#include "./World.hpp"

enum class BounceDirection {
  NONE,
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
};

class Bullet {
 public:
  Bullet(World* world, float x, float y, float angle, float speed, int health);

  bool getErase() const;
  void update(const float deltaTime);
  void draw() const;

  float getX() const;
  float getY() const;

  float getYVelocity() const;
  float getXVelocity() const;

  void bounce(BounceDirection direction);
  void destroy();

  void reverseDirection(const std::string& direction);

 private:
  World* worldPointer;

  asw::Vec2<float> position;
  float vector_x;
  float vector_y;

  int health;
  BounceDirection incidenceDirection = BounceDirection::NONE;

  bool pendingErase = false;
};

#endif  // SRC_GAME_BULLET_H_
