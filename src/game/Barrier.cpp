#include "Barrier.hpp"

#include <memory>

#include "../system/ImageRegistry.hpp"
#include "../system/SampleRegistry.hpp"
#include "./Particle.hpp"
#include "./PowerUp.hpp"

Barrier::Barrier(asw::scene::Scene<States>* scene,
                 const asw::Vec2<float>& position,
                 BarrierType type)
    : scene(scene) {
  transform = asw::Quad<float>(position.x, position.y, 0, 0);

  switch (type) {
    case BarrierType::BOX:
      image = ImageRegistry::getImage("block-box");
      health = 3;
      break;
    default:
      image = ImageRegistry::getImage("block-stone");
      health = 20;
      break;
  }

  transform.size = asw::util::getTextureSize(image);

  zIndex = 10;
}

// Update
void Barrier::update(float deltaTime) {
  if (health <= 0) {
    explode();
  }
}

// Draw image
void Barrier::draw() {
  if (health > 0) {
    asw::draw::sprite(image, transform.position);
  }
}

// Get width
float Barrier::getWidth() const {
  return transform.size.x;
}

// Get height
float Barrier::getHeight() const {
  return transform.size.y;
}

asw::Vec2<float> Barrier::getPosition() const {
  return this->transform.position;
}

// Explode
void Barrier::explode() {
  alive = false;

  // Explode
  asw::sound::play(SampleRegistry::getSample("explode"), 255, 127, 0);

  for (int i = 0; i < 100; i++) {
    const asw::Color color =
        asw::util::makeColor(255, asw::random::between(0, 255), 0);

    // Make particle
    scene->createObject<Particle>(scene, transform.getCenter(), color, -6.0,
                                  6.0, -6.0, 6.0, 2, ParticleType::SQUARE, 30,
                                  ParticleBehaviour::EXPLODE);
  }

  // Remove broken barriers
  if (asw::random::between(0, 1) == 0) {
    const int randomType = asw::random::between(0, 3);
    PowerUpType type;

    switch (randomType) {
      case 1:
        type = PowerUpType::SPEED;
        break;
      case 2:
        type = PowerUpType::FIRE_SPEED;
        break;
      case 3:
        type = PowerUpType::FIRE_DELAY;
        break;
      default:
        type = PowerUpType::HEALTH;
        break;
    }

    scene->createObject<PowerUp>(transform.position.x, transform.position.y,
                                 type);
  }
}
