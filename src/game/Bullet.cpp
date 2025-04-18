#include "./Bullet.hpp"

#include <cmath>

#include "../system/ImageRegistry.hpp"
#include "./Barrier.hpp"

// Init
Bullet::Bullet(asw::scene::Scene<States>* scene,
               float x,
               float y,
               float angle,
               float speed,
               int health,
               int team)
    : scene(scene),
      velocity(-speed * cosf(angle), -speed * sinf(angle)),
      team(team),
      health(health) {
  transform.position.x = x;
  transform.position.y = y;
  transform.size.x = 5;
  transform.size.y = 5;
  zIndex = 1;
  light_buffer = ImageRegistry::getImage("light");
}

// Reverse specified vector
void Bullet::reverseDirection(const std::string& direction) {
  if (direction == "x") {
    velocity.x = -velocity.x;
  } else if (direction == "y") {
    velocity.y = -velocity.y;
  } else {
    velocity.y = -velocity.y;
    velocity.x = -velocity.x;
  }
}

// Bounce off wall
void Bullet::bounce(BounceDirection direction) {
  health--;
  incidenceDirection = direction;
}

// Destroy
void Bullet::destroy() {
  // Has it already died?
  if (!alive) {
    return;
  }

  // Make sure health is 0
  health = 0;
  alive = false;

  // Make explosion
  for (int i = 0; i < 100; i++) {
    auto color = asw::util::makeColor(255, asw::random::between(0, 255), 0);

    switch (incidenceDirection) {
      case BounceDirection::BOTTOM: {
        scene->createObject<Particle>(scene, transform.getCenter(), color, -5,
                                      5, 0, 3, 2, ParticleType::SQUARE, 10,
                                      ParticleBehaviour::EXPLODE);
        break;
      }
      case BounceDirection::TOP: {
        scene->createObject<Particle>(scene, transform.getCenter(), color, -5,
                                      5, -3, 0, 2, ParticleType::SQUARE, 10,
                                      ParticleBehaviour::EXPLODE);
        break;
      }
      case BounceDirection::LEFT: {
        scene->createObject<Particle>(scene, transform.getCenter(), color, -3,
                                      0, -5, 5, 2, ParticleType::SQUARE, 10,
                                      ParticleBehaviour::EXPLODE);
        break;
      }
      default: {
        scene->createObject<Particle>(scene, transform.getCenter(), color, 0, 3,
                                      -5, 5, 2, ParticleType::SQUARE, 10,
                                      ParticleBehaviour::EXPLODE);
        break;
      }
    }
  }
}

// Update bullets
void Bullet::update(float deltaTime) {
  // Destroy if out of bounds or health is 0
  if (health <= 0 || transform.position.x < 0 || transform.position.x > 10000 ||
      transform.position.y < 0 || transform.position.y > 10000) {
    destroy();
  }

  // Move
  transform.position += velocity * (deltaTime / 8.0F);

  // Bounce
  for (auto& obj : scene->getObjectView<Barrier>()) {
    if (transform.collides(obj->transform)) {
      if (transform.collidesBottom(obj->transform)) {
        reverseDirection("y");
        bounce(BounceDirection::BOTTOM);
      } else if (transform.collidesTop(obj->transform)) {
        reverseDirection("y");
        bounce(BounceDirection::TOP);
      }

      if (transform.collidesLeft(obj->transform)) {
        reverseDirection("x");
        bounce(BounceDirection::LEFT);
      } else if (transform.collidesRight(obj->transform)) {
        reverseDirection("x");
        bounce(BounceDirection::RIGHT);
      }

      obj->hit();
    }
  }
}

// Draw image
void Bullet::draw() {
  asw::draw::rectFill(transform, asw::util::makeColor(0, 0, 0));
  const auto inner = transform + asw::Quad<float>(1, 1, -1, -1);
  asw::draw::rectFill(inner, asw::util::makeColor(255, 0, 0));
}
