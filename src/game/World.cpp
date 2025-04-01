#include "World.hpp"

#include <algorithm>
#include <iostream>

// Adds particle to global particle handler
void World::addParticle(std::shared_ptr<Particle> particle) {
  particles.emplace_back(particle);
}

// Updates world
void World::update(float deltaTime) {
  for (auto const& particle : particles) {
    particle->update(deltaTime);
  }

  particles.erase(
      std::remove_if(particles.begin(), particles.end(),
                     [](auto const& particle) { return particle->getDead(); }),
      particles.end());
}

// Draw world
void World::draw() const {
  // Draw particles
  for (auto const& particle : particles) {
    particle->draw();
  }
}
