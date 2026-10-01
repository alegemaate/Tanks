#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../state/State.hpp"

unsigned char Tank::num_bullet_bounces = 0;

// Speed a stopped tank starts at, and the speed it stops below (px/s)
constexpr float START_SPEED = 25.0F;
constexpr float STOP_SPEED = 12.5F;

// Exponential speed up and slow down rates (per second)
constexpr float ACCELERATION = 3.7F;
constexpr float DECELERATION = 6.1F;

Tank::Tank(asw::scene::Scene<States>* scene,
           asw::Camera& camera,
           const asw::Vec2<float>& position,
           int health,
           float fireSpeed,
           float fireDelay,
           float speed,
           int team)
    : health(health),
      initialHealth(health),
      fire_speed(fireSpeed),
      fire_delay_rate(fireDelay),
      max_speed(speed),
      image_base(nullptr),
      image_hurt(nullptr),
      image_top(nullptr),
      image_treads(nullptr),
      scene(scene),
      camera(camera),
      team(team) {
  transform.position = position;

  // Map size
  auto screenSize = asw::display::get_logical_size();
  map_width = screenSize.x;
  map_height = screenSize.y;
  z_index = 5;
}

// Explode
void Tank::explode() {
  const auto center = transform.get_center();

  for (int i = 0; i < 200; i++) {
    scene->create_object<Particle>(
        scene, center, asw::Color(255, asw::random::between(0, 255), 0),
        -250.0F, 250.0F, -250.0F, 250.0F, 4, ParticleType::SQUARE, 1.6F,
        ParticleBehaviour::FIRE);
  }

  asw::sound::play_positional(asw::assets::get_sample("tank-explode"), center);

  if (camera.get_view().contains(center)) {
    camera.shake(8.0F);
  }
}

void Tank::accelerate(bool moving, float dt) {
  if (moving) {
    if (speed < STOP_SPEED) {
      speed = START_SPEED;
    } else {
      speed = std::min(speed * std::exp(ACCELERATION * dt), max_speed);
    }
  } else {
    if (speed > STOP_SPEED) {
      speed *= std::exp(-DECELERATION * dt);
    } else {
      speed = 0;
    }
  }
}

// Check collision
void Tank::collideBullets(float dt) {
  for (auto& obj : scene->get_object_view<Bullet>()) {
    if (!obj->alive || obj->getTeam() == team) {
      continue;
    }

    const auto objTrans =
        obj->transform + asw::Quad<float>(obj->body.velocity * dt, {0, 0});

    if (transform.collides(objTrans)) {
      health -= 10;
      obj->destroy();
    }
  }
}

void Tank::collideBarriers(float dt) {
  const auto guess = asw::Vec2<float>::from_angle(rotation_body, -speed * dt);
  const auto offsetXPos = transform + asw::Quad<float>(2 + guess.x, 2, -2, -2);
  const auto offsetYPos = transform + asw::Quad<float>(2, 2 + guess.y, -2, -2);

  canMoveX = true;
  canMoveY = true;

  for (auto& obj : scene->get_object_view<Barrier>()) {
    if (offsetXPos.collides(obj->transform)) {
      canMoveX = false;
    }
    if (offsetYPos.collides(obj->transform)) {
      canMoveY = false;
    }
  }
}

void Tank::collidePowerUps() {
  for (auto& obj : scene->get_object_view<PowerUp>()) {
    if (obj->alive && transform.collides(obj->transform)) {
      pickupPowerUp(obj->getType());
      obj->pickup();
    }
  }
}

// Move around
void Tank::drive(float rotation, float dt) {
  const auto delta = asw::Vec2<float>::from_angle(rotation, -speed * dt);

  if (canMoveX) {
    transform.position.x += delta.x;
  }
  if (canMoveY) {
    transform.position.y += delta.y;
  }
}

// Shoot
void Tank::shoot(float rotation, const asw::Vec2<float>& target) {
  if (bullet_delay > fire_delay_rate) {
    asw::sound::play_positional(asw::assets::get_sample("fire"), target,
                                {.pitch_variation = 0.1F});

    scene->create_object<Bullet>(scene, target.x, target.y, rotation,
                                 fire_speed, 1 + num_bullet_bounces, team);

    bullet_delay = 0.0F;
  }
}

// Update
void Tank::update(float dt) {
  if (dead) {
    return;
  }

  // Collides
  collidePowerUps();
  collideBarriers(dt);
  collideBullets(dt);

  // Just died, leave a wreck under the living tanks
  if (health <= 0) {
    explode();
    dead = true;
    speed = 0;
    z_index = 4;
    return;
  }

  bullet_delay += dt;
}

// Draw Tank
void Tank::drawTankBase() {
  // Wreck
  if (dead) {
    asw::draw::rotate_sprite(image_hurt, transform.position, rotation_body);
  } else {
    asw::draw::rotate_sprite(image_base, transform.position, rotation_body);
  }
}

// Draw turret
void Tank::drawTankTurret() {
  if (dead) {
    return;
  }

  asw::draw::rotate_sprite(image_top, transform.position, rotation_turret);
}

// Draw health
void Tank::drawHealthBar(float x,
                         float y,
                         int width,
                         int height,
                         int border) const {
  if (health >= initialHealth || dead) {
    return;
  }

  const float healthPercent =
      std::clamp(static_cast<float>(health) / static_cast<float>(initialHealth),
                 0.0F, 1.0F);
  const auto inner = asw::Quad<float>(
      x + border, y + border, width - (border * 2), height - (border * 2));

  asw::draw::rect_fill(asw::Quad<float>(x, y, width, height),
                       asw::color::black);
  asw::draw::rect_fill(inner, asw::color::red);
  asw::draw::rect_fill(
      asw::Quad<float>(inner.position.x, inner.position.y,
                       inner.size.x * healthPercent, inner.size.y),
      asw::color::lime);
}

// Draw
void Tank::draw() {
  // Tank
  drawTankBase();

  // Turret
  drawTankTurret();

  // Health bar
  drawHealthBar(transform.position.x - 5, transform.position.y - 10, 50, 6, 1);
}

// Put decals
void Tank::putDecal() {
  if (!dead && speed > 0) {
    // Tread strip across the hull, centred on the tank
    const auto center = transform.get_center();
    const auto size = asw::util::get_texture_size(image_treads);
    asw::draw::rotate_sprite(image_treads, center - (size / 2.0F),
                             rotation_body);
  }
}

// Power ups
void Tank::pickupPowerUp(PowerUpType type) {
  switch (type) {
    case PowerUpType::HEALTH:
      health = std::min(health + 25, initialHealth);
      break;
    case PowerUpType::SPEED:
      max_speed += 31.25F;
      break;
    case PowerUpType::FIRE_SPEED:
      fire_speed += 125.0F;
      break;
    case PowerUpType::FIRE_DELAY:
      fire_delay_rate = std::max(fire_delay_rate - 0.1F, 0.01F);
      break;
    default:
      break;
  }
}
