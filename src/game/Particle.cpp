#include "Particle.hpp"

// Constructor
Particle::Particle(float x,
                   float y,
                   asw::Color color,
                   float xVelocityMin,
                   float xVelocityMax,
                   float yVelocityMin,
                   float yVelocityMax,
                   int size,
                   ParticleType type,
                   int life,
                   ParticleBehaviour behaviour)
    : transform(x, y, size, size),
      color(color),
      type(type),
      life(life),
      behaviour(behaviour) {
  this->velocity.x = asw::random::between(xVelocityMin, xVelocityMax);
  this->velocity.y = asw::random::between(yVelocityMin, yVelocityMax);

  // No unmoving
  if (velocity.x < 0.1f && velocity.x > -0.1f) {
    this->velocity.x = 0.1f;
  }
  if (velocity.y < 0.1f && velocity.y > -0.1f) {
    this->velocity.y = 0.1f;
  }
}

// Logic
void Particle::update(const float deltaTime) {
  auto deltaVelocity = velocity * (deltaTime / 8.0f);

  // Behaviour
  if (behaviour == ParticleBehaviour::EXPLODE) {
    transform.position += deltaVelocity;
    velocity -= deltaVelocity / 10.0f;
  } else {
    transform.position.x += asw::random::between(-velocity.x, velocity.x);
    transform.position.y += asw::random::between(-velocity.y, velocity.y);
  }

  // Die
  if (asw::random::between(0, life) < (deltaTime / 8.0f)) {
    dead = true;
  }
}

// Check death
bool Particle::getDead() const {
  return dead;
}

// Draw
void Particle::draw() const {
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
