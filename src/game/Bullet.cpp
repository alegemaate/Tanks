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

// Reverse specified vector
void Bullet::reverseDirection(const std::string& direction) {
  if (direction == "x") {
    body.velocity.x = -body.velocity.x;
  } else if (direction == "y") {
    body.velocity.y = -body.velocity.y;
  } else {
    body.velocity *= -1.0F;
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
        scene->create_object<Particle>(scene, center, color, -625, 625, -375,
                                       0, 2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
      case BounceDirection::LEFT: {
        scene->create_object<Particle>(scene, center, color, -375, 0, -625,
                                       625, 2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
      default: {
        scene->create_object<Particle>(scene, center, color, 0, 375, -625,
                                       625, 2, ParticleType::SQUARE, 0.09F,
                                       ParticleBehaviour::EXPLODE);
        break;
      }
    }
  }
}

// Update bullets
void Bullet::update(float dt) {
  // Destroy if out of bounds or health is 0
  if (health <= 0 || transform.position.x < 0 || transform.position.x > 10000 ||
      transform.position.y < 0 || transform.position.y > 10000) {
    destroy();
  }

  // Move
  GameObject::update(dt);

  // Bounce
  for (auto& obj : scene->get_object_view<Barrier>()) {
    if (transform.collides(obj->transform)) {
      if (transform.collides_bottom(obj->transform)) {
        reverseDirection("y");
        bounce(BounceDirection::BOTTOM);
      } else if (transform.collides_top(obj->transform)) {
        reverseDirection("y");
        bounce(BounceDirection::TOP);
      }

      if (transform.collides_left(obj->transform)) {
        reverseDirection("x");
        bounce(BounceDirection::LEFT);
      } else if (transform.collides_right(obj->transform)) {
        reverseDirection("x");
        bounce(BounceDirection::RIGHT);
      }

      obj->hit();
    }
  }
}

// Draw image
void Bullet::draw() {
  asw::draw::rect_fill(transform, asw::color::black);
  const auto inner = transform + asw::Quad<float>(1, 1, -1, -1);
  asw::draw::rect_fill(inner, asw::color::red);
}
