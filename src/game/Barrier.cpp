#include "Barrier.hpp"

#include <memory>

#include "../system/ImageRegistry.hpp"

asw::Sample Barrier::sample_explode;

Barrier::Barrier(World* world,
                 const asw::Vec2<float>& position,
                 BarrierType type)
    : transform(position.x, position.y, 0, 0), worldPointer(world) {
  switch (type) {
    case BarrierType::BOX:
      this->image = ImageRegistry::getImage("block-box");
      this->health = 3;
      break;
    default:
      this->image = ImageRegistry::getImage("block-stone");
      this->health = 20;
      break;
  }

  transform.size = asw::util::getTextureSize(this->image);

  if (Barrier::sample_explode == nullptr) {
    Barrier::sample_explode = asw::assets::loadSample("assets/sfx/explode.wav");
  }
}

// Update
void Barrier::update(std::vector<Bullet>& bullets) {
  if (health <= 0 && !exploded) {
    explode();
    exploded = true;
  }

  if (health <= 0) {
    return;
  }

  for (auto& bullet : bullets) {
    if (collisionAny(transform.position.x,
                     transform.position.x + transform.size.x, bullet.getX(),
                     bullet.getX() + 5, transform.position.y,
                     transform.position.y + transform.size.y, bullet.getY(),
                     bullet.getY() + 5)) {
      if (collisionBottom(bullet.getY() + bullet.getYVelocity(),
                          bullet.getY() + 5,
                          transform.position.y + transform.size.y)) {
        bullet.reverseDirection("y");
        bullet.bounce(BounceDirection::BOTTOM);
      }

      if (collisionTop(bullet.getY(), bullet.getY() + 5 + bullet.getYVelocity(),
                       transform.position.y)) {
        bullet.reverseDirection("y");
        bullet.bounce(BounceDirection::TOP);
      }

      if (collisionLeft(bullet.getX(),
                        bullet.getX() + 5 + bullet.getXVelocity(),
                        transform.position.x)) {
        bullet.reverseDirection("x");
        bullet.bounce(BounceDirection::LEFT);
      }

      if (collisionRight(bullet.getX() + bullet.getXVelocity(),
                         bullet.getX() + 5,
                         transform.position.x + transform.size.x)) {
        bullet.reverseDirection("x");
        bullet.bounce(BounceDirection::RIGHT);
      }

      if (!indestructible) {
        health -= 1;
      }
    }
  }
}

// Draw image
void Barrier::draw() const {
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

// Check if it needs cleanup
bool Barrier::getDead() const {
  return !indestructible && health <= 0;
}

asw::Vec2<float> Barrier::getPosition() const {
  return this->transform.position;
}

// Explode
void Barrier::explode() {
  // Explode
  asw::sound::play(sample_explode, 255, 127, 0);

  for (int i = 0; i < 100; i++) {
    asw::Color color =
        asw::util::makeColor(255, asw::random::between(0, 255), 0);

    // Make sure not transparent (they show as white)
    // do { TODO
    //   // position of colour
    //   int randomY = asw::random::between(0, static_cast<int>(height));
    //   int randomX = asw::random::between(0, static_cast<int>(width));

    //   // New colour
    //   color = getpixel(image, randomY, randomX);
    // } while (getr(color) == 255 && getg(color) == 255 && getb(color) == 255);

    // Make particle
    auto particle = std::make_shared<Particle>(
        transform.position.x + transform.size.x / 2.0f,
        transform.position.y + transform.size.y / 2.0f, color, -6.0, 6.0, -6.0,
        6.0, 2, ParticleType::SQUARE, 30, ParticleBehaviour::EXPLODE);

    worldPointer->addParticle(particle);
  }
}
