#ifndef SRC_GAME_BARRIER_H_
#define SRC_GAME_BARRIER_H_

#include <asw/asw.h>
#include <vector>

#include "./Bullet.hpp"

enum class BarrierType {
  NONE,
  BOX,
  STONE,
};

class Barrier {
 public:
  Barrier(World* world, const asw::Vec2<float>& position, BarrierType type);

  void update(std::vector<Bullet>& bullets);
  void draw() const;

  float getWidth() const;
  float getHeight() const;

  void makeIndestructible(bool i) { this->indestructible = i; }

  bool getDead() const;

  asw::Vec2<float> getPosition() const;

 private:
  asw::Quad<float> transform;

  World* worldPointer;

  int health;

  bool indestructible = false;
  bool exploded = false;

  static asw::Sample sample_explode;

  asw::Texture image;

  void explode();
};

#endif  // SRC_GAME_BARRIER_H_
