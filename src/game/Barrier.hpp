#ifndef SRC_GAME_BARRIER_H_
#define SRC_GAME_BARRIER_H_

#include <asw/asw.h>
#include <vector>

#include "../state/State.hpp"

enum class BarrierType {
  NONE,
  BOX,
  STONE,
};

class Barrier : public asw::game::GameObject {
 public:
  Barrier(asw::scene::Scene<States>* scene,
          const asw::Vec2<float>& position,
          BarrierType type);

  void update(float deltaTime) override;
  void draw() override;

  float getWidth() const;
  float getHeight() const;

  void hit() {
    if (indestructible) {
      return;
    }
    health--;
  }

  void makeIndestructible(bool i) { this->indestructible = i; }

  asw::Vec2<float> getPosition() const;

 private:
  asw::scene::Scene<States>* scene;
  int health;
  bool indestructible = false;
  asw::Texture image;

  void explode();
};

#endif  // SRC_GAME_BARRIER_H_
