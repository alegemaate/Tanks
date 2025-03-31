#include "./Bullet.hpp"

#include <cmath>

// Init
Bullet::Bullet(World* world,
               float x,
               float y,
               float angle,
               float speed,
               int health)
    : worldPointer(world), position(x, y), health(health) {
  this->vector_x = -speed * cosf(angle);
  this->vector_y = -speed * sinf(angle);
}

bool Bullet::getErase() const {
  return pendingErase;
}

// Reverse specified vector
void Bullet::reverseDirection(const std::string& direction) {
  if (direction == "x") {
    vector_x = -vector_x;
  } else if (direction == "y") {
    vector_y = -vector_y;
  } else {
    vector_y = -vector_y;
    vector_x = -vector_x;
  }
}

// Get coordinates
float Bullet::getX() const {
  return position.x;
}

float Bullet::getY() const {
  return position.y;
}

// Bounce off wall
void Bullet::bounce(BounceDirection direction) {
  health--;
  incidenceDirection = direction;

  if (health <= 0) {
    destroy();
  }
}

// Destroy
void Bullet::destroy() {
  // Has it already died?
  if (pendingErase) {
    return;
  }

  // Make sure health is 0
  health = 0;
  pendingErase = true;

  // Make explosion
  for (int i = 0; i < 100; i++) {
    auto color = asw::util::makeColor(255, asw::random::between(0, 255), 0);

    switch (incidenceDirection) {
      case BounceDirection::BOTTOM: {
        auto particle = std::make_shared<Particle>(
            position.x, position.y, color, -5, 5, 0, 3, 2, ParticleType::SQUARE,
            10, ParticleBehaviour::EXPLODE);

        worldPointer->addParticle(particle);
        break;
      }
      case BounceDirection::TOP: {
        auto particle = std::make_shared<Particle>(
            position.x, position.y, color, -5, 5, -3, 0, 2,
            ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
        worldPointer->addParticle(particle);
        break;
      }
      case BounceDirection::LEFT: {
        auto particle = std::make_shared<Particle>(
            position.x, position.y, color, -3, 0, -5, 5, 2,
            ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
        worldPointer->addParticle(particle);
        break;
      }
      default: {
        auto particle = std::make_shared<Particle>(
            position.x, position.y, color, 0, 3, -5, 5, 2, ParticleType::SQUARE,
            10, ParticleBehaviour::EXPLODE);
        worldPointer->addParticle(particle);
        break;
      }
    }
  }
}

// Return velocities
float Bullet::getXVelocity() const {
  return vector_x;
}

float Bullet::getYVelocity() const {
  return vector_y;
}

// Update bullets
void Bullet::update(const float deltaTime) {
  if (health > 0) {
    // Move
    position += asw::Vec2<float>(vector_x, vector_y) * (deltaTime / 8.0f);

    // Off screen
    if (position.x < 0 || position.x > 10000 || position.y < 0 ||
        position.y > 10000) {
      destroy();
    }
  }
}

// Draw image
void Bullet::draw() const {
  auto x_int = static_cast<int>(position.x);
  auto y_int = static_cast<int>(position.y);

  if (health > 0) {
    asw::draw::rectFill(asw::Quad<float>(x_int, y_int, 5, 5),
                        asw::util::makeColor(0, 0, 0));
    asw::draw::rectFill(asw::Quad<float>(x_int + 1, y_int + 1, 3, 3),
                        asw::util::makeColor(255, 0, 0));
    asw::draw::rectFill(asw::Quad<float>(x_int + 2, y_int + 2, 1, 1),
                        asw::util::makeColor(0, 255, 0));
  }
}
