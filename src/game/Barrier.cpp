#include "Barrier.hpp"

#include <memory>

#include "./Particle.hpp"
#include "./PowerUp.hpp"

Barrier::Barrier(asw::scene::Scene<States>* scene,
                 const asw::Vec2<float>& position,
                 BarrierType type)
    : scene(scene) {
  transform = asw::Quad<float>(position.x, position.y, 0, 0);

  switch (type) {
    case BarrierType::BOX:
      image = asw::assets::get_texture("block-box");
      health = 3;
      break;
    default:
      image = asw::assets::get_texture("block-stone");
      health = 20;
      break;
  }

  transform.size = asw::util::get_texture_size(image);

  z_index = 10;
}

// Update
void Barrier::update(float /*dt*/) {
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
  const auto center = transform.get_center();
  asw::sound::play_positional(asw::assets::get_sample("explode"), center);

  for (int i = 0; i < 100; i++) {
    const asw::Color color(255, asw::random::between(0, 255), 0);

    // Make particle
    scene->create_object<Particle>(scene, center, color, -750.0F, 750.0F,
                                   -750.0F, 750.0F, 2, ParticleType::SQUARE,
                                   0.25F, ParticleBehaviour::EXPLODE);
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

    scene->create_object<PowerUp>(transform.position.x, transform.position.y,
                                  type);
  }
}
