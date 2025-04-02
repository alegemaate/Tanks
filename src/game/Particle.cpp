#include "Particle.hpp"

// Constructor
Particle::Particle(const asw::Vec2<float>& position,
                   asw::Color color,
                   float xVelocityMin,
                   float xVelocityMax,
                   float yVelocityMin,
                   float yVelocityMax,
                   int size,
                   ParticleType type,
                   int life,
                   ParticleBehaviour behaviour)
    : color(color), type(type), life(life), behaviour(behaviour) {
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
  } else {
    transform.position.x += asw::random::between(-velocity.x, velocity.x);
    transform.position.y += asw::random::between(-velocity.y, velocity.y);
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
    case ParticleType::RANDOM:
      drawRandom();
      break;
    default:
      break;
  }
}

void Particle::drawRandom() const {
  switch (asw::random::between(0, 3)) {
    case 0:
      asw::draw::point(transform.position, color);
      break;
    case 1:
      asw::draw::circleFill(transform.position, transform.size.x, color);
      break;
    case 2:
      asw::draw::rectFill(transform, color);
      break;
    default:
      break;
  }
}
