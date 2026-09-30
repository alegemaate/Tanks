#include "./Bullet.hpp"

#include <cmath>

#include "./Barrier.hpp"

// Init
Bullet::Bullet(asw::scene::Scene<States>* scene,
               float x,
               float y,
               float angle,
               float speed,
               int health,
               int team)
    : scene(scene), team(team), health(health) {
  transform.position.x = x;
  transform.position.y = y;
  transform.size.x = 5;
  transform.size.y = 5;
  z_index = 1;
  body.velocity = asw::Vec2<float>::from_angle(angle, -speed);
  light_buffer = asw::assets::get_texture("light");
}

// Damage every barrier the bullet overlaps, true if there were any
bool Bullet::hitBarriers() {
  bool hit = false;

  for (auto& obj : scene->get_object_view<Barrier>()) {
    if (obj->alive && transform.collides(obj->transform)) {
      obj->hit();
      hit = true;
    }
  }

  return hit;
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
  const auto center = transform.get_center();

  for (int i = 0; i < 100; i++) {
    auto color = asw::Color(255, asw::random::between(0, 255), 0);

    switch (incidenceDirection) {
      case BounceDirection::BOTTOM: {
        scene->create_object<Particle>(scene, center, color, -625, 625, 0, 375,
                                       2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
      case BounceDirection::TOP: {
        scene->create_object<Particle>(scene, center, color, -625, 625, -375, 0,
                                       2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
      case BounceDirection::LEFT: {
        scene->create_object<Particle>(scene, center, color, -375, 0, -625, 625,
                                       2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
      default: {
        scene->create_object<Particle>(scene, center, color, 0, 375, -625, 625,
                                       2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
    }
  }
}

// Update bullets
void Bullet::update(float dt) {
  // Destroy if out of bounds
  if (transform.position.x < 0 || transform.position.x > 10000 ||
      transform.position.y < 0 || transform.position.y > 10000) {
    destroy();
    return;
  }

  // Move one axis at a time, so the axis that hits a wall is the one that
  // bounces, also where two blocks meet
  const auto delta = body.velocity * dt;
  bool bounced = false;

  transform.position.x += delta.x;
  if (hitBarriers()) {
    transform.position.x -= delta.x;
    body.velocity.x = -body.velocity.x;
    incidenceDirection =
        delta.x > 0 ? BounceDirection::LEFT : BounceDirection::RIGHT;
    bounced = true;
  }

  transform.position.y += delta.y;
  if (hitBarriers()) {
    transform.position.y -= delta.y;
    body.velocity.y = -body.velocity.y;
    incidenceDirection =
        delta.y > 0 ? BounceDirection::TOP : BounceDirection::BOTTOM;
    bounced = true;
  }

  // Each bounce uses up one health, a corner counts once
  if (bounced && --health <= 0) {
    destroy();
  }
}

// Draw image
void Bullet::draw() {
  asw::draw::rect_fill(transform, asw::color::black);
  const auto inner = transform + asw::Quad<float>(1, 1, -2, -2);
  asw::draw::rect_fill(inner, asw::color::red);
}
