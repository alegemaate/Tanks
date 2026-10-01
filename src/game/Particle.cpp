#include "Particle.hpp"

#include <cmath>

#include "./Barrier.hpp"

// Velocity lost per second
constexpr float EXPLODE_DRAG = 13.2F;
constexpr float FIRE_DRAG = 6.4F;

// Slowest a particle may start (px/s)
constexpr float MIN_SPEED = 12.5F;

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
                   float life,
                   ParticleBehaviour behaviour)
    : scene(scene), color(color), type(type), life(life), behaviour(behaviour) {
  transform = asw::Quad<float>(position.x, position.y, size, size);
  body.velocity.x = asw::random::between(xVelocityMin, xVelocityMax);
  body.velocity.y = asw::random::between(yVelocityMin, yVelocityMax);
  light_buffer = asw::assets::get_texture("light");

  // No unmoving
  if (std::abs(body.velocity.x) < MIN_SPEED) {
    body.velocity.x = MIN_SPEED;
  }

  if (std::abs(body.velocity.y) < MIN_SPEED) {
    body.velocity.y = MIN_SPEED;
  }

  if (behaviour == ParticleBehaviour::EXPLODE) {
    z_index = asw::random::between(0, 9);
  } else {
    z_index = 0;
  }
}

// Logic
void Particle::update(float dt) {
  // Move
  GameObject::update(dt);

  // Behaviour
  const float drag =
      behaviour == ParticleBehaviour::EXPLODE ? EXPLODE_DRAG : FIRE_DRAG;
  body.velocity *= std::exp(-drag * dt);

  // Die on collision
  // for (auto& obj : scene->get_object_view<Barrier>()) {
  //   if (transform.collides(obj->transform)) {
  //     alive = false;
  //   }
  // }

  // Die
  if (asw::random::chance(dt / life)) {
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
      asw::draw::rect_fill(transform, color);
      break;
    case ParticleType::CIRCLE:
      asw::draw::circle_fill(transform.position, transform.size.x, color);
      break;
    default:
      break;
  }
}
