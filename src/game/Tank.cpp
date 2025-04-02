#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../state/State.hpp"
#include "../system/ImageRegistry.hpp"

unsigned char Tank::num_bullet_bounces = 0;
asw::Sample Tank::sample_shot;

Tank::Tank(asw::scene::Scene<States>* scene,
           const asw::Vec2<float>& position,
           int health,
           int fireSpeed,
           int fireDelay,
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
      team(team) {
  transform.position = position;

  // Map size
  auto screenSize = asw::display::getSize();
  map_width = screenSize.x;
  map_height = screenSize.y;

  if (Tank::sample_shot == nullptr) {
    Tank::sample_shot = asw::assets::loadSample("assets/sfx/fire.wav");
  }
}

// Explode
void Tank::explode() {
  for (int i = 0; i < 200; i++) {
    scene->createObject<Particle>(
        transform.getCenter(),
        asw::util::makeColor(255, asw::random::between(0, 255), 0), -10.0F,
        10.0F, -10.0F, 10.0F, 2, ParticleType::SQUARE, 20,
        ParticleBehaviour::EXPLODE);
  }
}

void Tank::accelerate(bool moving, float deltaTime) {
  if (moving) {
    if (speed < 0.1F) {
      speed = 0.2F;
    } else if (speed < max_speed) {
      speed *= (max_speed * 1.03F) * (deltaTime / 8.0F);
    } else {
      speed = max_speed;
    }
  } else {
    if (speed > 0.1F) {
      speed /= 1.05F * (deltaTime / 8.0F);
    } else {
      speed = 0;
    }
  }
}

// Check collision
void Tank::collideBullets(float deltaTime) {
  for (auto& obj : scene->getObjectView<Bullet>()) {
    if (obj->getTeam() == team) {
      continue;
    }

    const auto objTrans =
        obj->transform +
        asw::Quad<float>(obj->getXVelocity() * (deltaTime / 8.0F),
                         obj->getYVelocity() * (deltaTime / 8.0F), 0, 0);

    if (transform.contains(objTrans)) {
      health -= 10;
      obj->destroy();
    }
  }
}

void Tank::collideBarriers(float deltaTime) {
  const float delta_speed = speed * (deltaTime / 8.0F);
  const float guess_vector_x = -delta_speed * cosf(rotation_body);
  const float guess_vector_y = -delta_speed * sinf(rotation_body);
  const auto offsetXPos =
      transform + asw::Quad<float>(2 + guess_vector_x, 2, -2, -2);
  const auto offsetYPos =
      transform + asw::Quad<float>(2, 2 + guess_vector_y, -2, -2);

  canMoveX = true;
  canMoveY = true;

  for (auto& obj : scene->getObjectView<Barrier>()) {
    if (offsetXPos.contains(obj->transform)) {
      canMoveX = false;
    }
    if (offsetYPos.contains(obj->transform)) {
      canMoveY = false;
    }
  }
}

void Tank::collidePowerUps() {
  for (auto& obj : scene->getObjectView<PowerUp>()) {
    if (transform.contains(obj->transform)) {
      pickupPowerUp(obj->getType());
      obj->pickup();
    }
  }
}

// Move around
void Tank::drive(float rotation, float deltaTime) {
  float deltaSpeed = speed * (deltaTime / 8.0f);

  if (canMoveX) {
    transform.position.x += -deltaSpeed * cosf(rotation);
  }
  if (canMoveY) {
    transform.position.y += -deltaSpeed * sinf(rotation);
  }
}

// Shoot
void Tank::shoot(float rotation, const asw::Vec2<float>& target) {
  if (bullet_delay > fire_delay_rate) {
    asw::sound::play(sample_shot, 255, 127, 0);

    scene->createObject<Bullet>(scene, target.x, target.y, rotation, fire_speed,
                                1 + num_bullet_bounces, team);

    bullet_delay = 0;
  }
}

// Update
void Tank::update(float deltaTime) {
  // Collides
  collidePowerUps();
  collideBarriers(deltaTime);
  collideBullets(deltaTime);

  // Just died
  if (alive && (health <= 0)) {
    explode();
    asw::sound::play(sample_shot, 255, 127, 0);
    alive = false;
  }

  bullet_delay += deltaTime;
}

// Draw Tank
void Tank::drawTankBase() {
  // Hurt image for player
  if (!alive) {
    asw::draw::rotateSprite(image_hurt, transform.position,
                            rad_to_deg(rotation_body));
  } else {
    asw::draw::rotateSprite(image_base, transform.position,
                            rad_to_deg(rotation_body));
  }
}

// Draw turret
void Tank::drawTankTurret() {
  asw::draw::rotateSprite(image_top, transform.position,
                          rad_to_deg(rotation_turret));
}

// Draw health
void Tank::drawHealthBar(float x,
                         float y,
                         int width,
                         int height,
                         int border) const {
  const float healthPercent =
      static_cast<float>(health) / static_cast<float>(initialHealth);

  asw::draw::rectFill(asw::Quad<float>(x, y, width, height),
                      asw::util::makeColor(0, 0, 0));
  asw::draw::rectFill(
      asw::Quad<float>(x + border, y + border, width - border, height - border),
      asw::util::makeColor(255, 0, 0));
  asw::draw::rectFill(
      asw::Quad<float>(x + border, y + border, (healthPercent * width) - border,
                       height - border),
      asw::util::makeColor(0, 255, 0));
}

// Draw
void Tank::draw() {
  // Tank
  drawTankBase();

  // Turret
  if (alive) {
    drawTankTurret();

    // Health bar
    if (health < initialHealth) {
      drawHealthBar(transform.position.x - 5, transform.position.y - 10, 50, 6,
                    1);
    }
  }
}

// Put decals
void Tank::putDecal() {
  if (alive && speed > 0) {
    asw::draw::rotateSprite(
        image_treads,
        asw::Vec2<float>(transform.getCenter().x, transform.position.y),
        rad_to_deg(rotation_turret));
  }
}

// Power ups
void Tank::pickupPowerUp(PowerUpType type) {
  switch (type) {
    case PowerUpType::HEALTH:
      health += 25;
      if (health > 100) {
        health = 100;
      }
      break;
    case PowerUpType::SPEED:
      max_speed += 0.25F;
      break;
    case PowerUpType::FIRE_SPEED:
      fire_speed += 1;
      break;
    case PowerUpType::FIRE_DELAY:
      fire_delay_rate -= 100;
      if (fire_delay_rate < 10) {
        fire_delay_rate = 10;
      }
      break;
    default:
      break;
  }
}
