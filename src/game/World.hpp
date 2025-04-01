#ifndef SRC_GAME_WORLD_H_
#define SRC_GAME_WORLD_H_

#include <memory>
#include <vector>

#include "Particle.hpp"

class World {
 public:
  void update(float deltaTime);
  void draw() const;

  void addParticle(std::shared_ptr<Particle> particle);

 private:
  std::vector<std::shared_ptr<Particle>> particles;
};

#endif  // SRC_GAME_WORLD_H_
