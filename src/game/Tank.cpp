#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../system/ImageRegistry.hpp"

unsigned char Tank::num_bullet_bounces = 0;
asw::Sample Tank::sample_shot;

Tank::Tank(World* worldPointer,
           float x,
           float y,
           int health,
           int fireSpeed,
           int fireDelay,
           float speed)
    : position(x, y),
      health(health),
      initialHealth(health),
      fire_speed(fireSpeed),
      fire_delay_rate(fireDelay),
      max_speed(speed),
      image_base(nullptr),
      image_hurt(nullptr),
      image_top(nullptr),
      image_treads(nullptr),
      worldPointer(worldPointer) {
  // Map size
  auto screenSize = asw::display::getSize();
  map_width = screenSize.x;
  map_height = screenSize.y;

  if (Tank::sample_shot == nullptr) {
    Tank::sample_shot = asw::assets::loadSample("assets/sfx/fire.wav");
  }
}

// Check dead
bool Tank::isDead() const {
  return dead;
}

// Explode
void Tank::explode() {
  for (int i = 0; i < 200; i++) {
    auto particle = std::make_shared<Particle>(
        getCenterX(), getCenterY(),
        asw::util::makeColor(255, asw::random::between(0, 255), 0), -10.0f,
        10.0f, -10.0f, 10.0f, 2, ParticleType::SQUARE, 20,
        ParticleBehaviour::EXPLODE);
    worldPointer->addParticle(particle);
  }
}

void Tank::accelerate(bool moving, float deltaTime) {
  if (moving) {
    if (speed < 0.1f) {
      speed = 0.2f;
    } else if (speed < max_speed) {
      speed *= (max_speed * 1.03f) * (deltaTime / 8.0f);
    } else {
      speed = max_speed;
    }
  } else {
    if (speed > 0.1f) {
      speed /= 1.05f * (deltaTime / 8.0f);
    } else {
      speed = 0;
    }
  }
}

// Get bullets
std::vector<Bullet>& Tank::getBullets() {
  return bullets;
}

// Check collision
void Tank::checkCollision(std::vector<Bullet>& enemyBullets, float deltaTime) {
  for (auto& bullet : enemyBullets) {
    if (collisionAny(
            position.x, position.x + 50, bullet.getX(),
            bullet.getX() + bullet.getXVelocity() * (deltaTime / 8.0f),
            position.y, position.y + 50, bullet.getY(),
            bullet.getY() + bullet.getYVelocity() * (deltaTime / 8.0f))) {
      health -= 10;
      bullet.destroy();
    }
  }
}

void Tank::checkCollision(const std::vector<Barrier>& barriers,
                          float deltaTime) {
  float delta_speed = speed * (deltaTime / 8.0f);
  float guess_vector_x = -delta_speed * cosf(rotation_body);
  float guess_vector_y = -delta_speed * sinf(rotation_body);

  canMoveX = true;
  canMoveY = true;

  for (const auto& barrier : barriers) {
    if (collisionAny(
            position.x + 2 + guess_vector_x,
            position.x + width - 2 + guess_vector_x, barrier.getPosition().x,
            barrier.getPosition().x + barrier.getWidth(), position.y + 2,
            position.y + height - 2, barrier.getPosition().y,
            barrier.getPosition().y + barrier.getHeight())) {
      canMoveX = false;
    }
    if (collisionAny(
            position.x + 2, position.x + width - 2, barrier.getPosition().x,
            barrier.getPosition().x + barrier.getWidth(),
            position.y + 2 + guess_vector_y,
            position.y + height - 2 + guess_vector_y, barrier.getPosition().y,
            barrier.getPosition().y + barrier.getHeight())) {
      canMoveY = false;
    }
  }
}

void Tank::checkCollision(std::vector<PowerUp>& power_ups, float deltaTime) {
  for (auto& power_up : power_ups) {
    if (collisionAny(position.x, position.x + 50, power_up.getX(),
                     power_up.getX() + power_up.getWidth(), position.y,
                     position.y + 50, power_up.getY(),
                     power_up.getY() + power_up.getHeight())) {
      pickupPowerUp(power_up.getType());
      power_up.pickup();
    }
  }
}

// Move around
void Tank::drive(float rotation, float deltaTime) {
  float deltaSpeed = speed * (deltaTime / 8.0f);

  if (canMoveX) {
    position.x += -deltaSpeed * cosf(rotation);
  }
  if (canMoveY) {
    position.y += -deltaSpeed * sinf(rotation);
  }
}

// Update bullets
void Tank::update_bullets(float deltaTime) {
  // Update bullets
  for (auto& bullet : bullets) {
    bullet.update(deltaTime);
  }

  // Erase bullets
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](auto const& bullet) { return bullet.getErase(); }),
      bullets.end());
}

// Shoot
void Tank::shoot(float rotation, float targetX, float targetY) {
  if (bullet_delay > fire_delay_rate) {
    asw::sound::play(sample_shot, 255, 127, 0);

    bullets.emplace_back(worldPointer, targetX, targetY, rotation, fire_speed,
                         1 + num_bullet_bounces);

    bullet_delay = 0;
  }
}

// Update
void Tank::update(float deltaTime) {
  // Just died
  if (!dead && (health < 1)) {
    explode();
    asw::sound::play(sample_shot, 255, 127, 0);
    dead = true;
  }

  bullet_delay += deltaTime;

  update_bullets(deltaTime);
}

// Draw bullets
void Tank::drawBullets() const {
  for (auto& bullet : bullets) {
    bullet.draw();
  }
}

// Draw Tank
void Tank::drawTankBase() {
  // Hurt image for player
  if (dead) {
    asw::draw::rotateSprite(image_hurt, position, rad_to_deg(rotation_body));
  } else {
    asw::draw::rotateSprite(image_base, position, rad_to_deg(rotation_body));
  }
}

// Draw turret
void Tank::drawTankTurret() {
  asw::draw::rotateSprite(image_top, position, rad_to_deg(rotation_turret));
}

// Draw health
void Tank::drawHealthBar(float x,
                         float y,
                         int width,
                         int height,
                         int border) const {
  float healthPercent =
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

  // Bullets
  drawBullets();

  // Turret
  if (!isDead()) {
    drawTankTurret();

    // Health bar
    if (health < initialHealth) {
      drawHealthBar(position.x - 5, position.y - 10, 50, 6, 1);
    }
  }
}

// Put decals
void Tank::putDecal() {
  if (!dead && speed > 0) {
    asw::draw::rotateSprite(image_treads,
                            asw::Vec2<float>(getCenterX(), position.y),
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
      max_speed += 0.25f;
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
