#include "Particle.hpp"

#include "./Barrier.hpp"

// Constructor
Particle::Particle(asw::scene::Scene<States>* scene,
                   const asw::Vec2<float>& position,
                   asw::Color color,
                   float xVelocityMin,
                   float xVelocityMax,
                   float yVelocityMin,
                   float yVelocityMax,
                   int size,
                   ParticleType type,
                   int life,
                   ParticleBehaviour behaviour)
    : scene(scene), color(color), type(type), life(life), behaviour(behaviour) {
  transform = asw::Quad<float>(position.x, position.y, size, size);
  velocity.x = asw::random::between(xVelocityMin, xVelocityMax);
  velocity.y = asw::random::between(yVelocityMin, yVelocityMax);

  // No unmoving
  if (velocity.x < 0.1F && velocity.x > -0.1F) {
    velocity.x = 0.1F;
  }

  if (velocity.y < 0.1F && velocity.y > -0.1F) {
    velocity.y = 0.1F;
  }
}

// Logic
void Particle::update(float deltaTime) {
  auto deltaVelocity = velocity * (deltaTime / 8.0F);

  // Behaviour
  if (behaviour == ParticleBehaviour::EXPLODE) {
    transform.position += deltaVelocity;
    velocity -= deltaVelocity / 10.0F;
  }

  // Bounce
  for (auto& obj : scene->getObjectView<Barrier>()) {
    if (transform.collides(obj->transform)) {
      alive = false;
    }
  }

  // Die
  if (asw::random::between(0, life) < (deltaTime / 8.0F)) {
    alive = false;
  }
}

// Draw
void Particle::draw() {
  switch (type) {
    case ParticleType::PIXEL:
      asw::draw::point(transform.position, color);
      break;
    case ParticleType::SQUARE:
      asw::draw::rectFill(transform, color);
      break;
    case ParticleType::CIRCLE:
      asw::draw::circleFill(transform.position, transform.size.x, color);
      break;
    default:
      break;
  }
}
