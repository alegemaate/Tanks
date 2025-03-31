#include "AiTank.hpp"

#include "../system/ImageRegistry.hpp"

// Init
AiTank::AiTank(World* world,
               float x,
               float y,
               int health,
               int fireSpeed,
               int fireDelay,
               float speed,
               bool enemy)
    : Tank(world, x, y, health, fireSpeed, fireDelay, speed) {
  if (enemy) {
    this->image_treads = ImageRegistry::getImage("tank-treads");
    this->image_hurt = ImageRegistry::getImage("tank-dead");
    this->image_top = ImageRegistry::getImage("tank-turret-red");
    this->image_base = ImageRegistry::getImage("tank-base-red");
  } else {
    this->image_treads = ImageRegistry::getImage("tank-treads");
    this->image_hurt = ImageRegistry::getImage("tank-dead");
    this->image_top = ImageRegistry::getImage("tank-turret-blue");
    this->image_base = ImageRegistry::getImage("tank-base-blue");
  }

  this->destination_x = x;
  this->destination_y = y;

  auto imageSize = asw::util::getTextureSize(image_base);
  this->width = static_cast<float>(imageSize.x);
  this->height = static_cast<float>(imageSize.y);
}

// Update
void AiTank::update(const float deltaTime) {
  Tank::update(deltaTime);

  if (dead) {
    return;
  }

  find_enemy_target();
  update_target();
  ai_drive(deltaTime);
}

void AiTank::find_enemy_target() {
  // Rotate turret (at closest enemy)
  float closest_enemy_x = destination_x;
  float closest_enemy_y = destination_y;
  bool found_enemy = false;

  for (const auto& tank : *otherTanks) {
    if (!tank->isDead()) {
      float enemy_x = tank->getX() + tank->getCenterX();
      float enemy_y = tank->getY() + tank->getCenterY();
      found_enemy = true;

      if (find_distance(position.x, position.y, enemy_x, enemy_y) <
          find_distance(position.x, position.y, closest_enemy_x,
                        closest_enemy_y)) {
        closest_enemy_x = enemy_x;
        closest_enemy_y = enemy_y;
      }
    }
  }

  if (found_enemy) {
    rotation_turret = find_angle(position.x + 25, position.y + 25,
                                 closest_enemy_x, closest_enemy_y);

    // Shoot
    float distanceToEnemy =
        find_distance(position.x, position.y, closest_enemy_x, closest_enemy_y);

    if (asw::random::between(0, 10) == 0 && distanceToEnemy < 500) {
      shoot(rotation_turret, position.x + 23, position.y + 23);
    }
  } else {
    rotation_turret = rotation_body;
  }
}

// Ai point choosing
void AiTank::update_target() {
  const float distanceToTarget =
      find_distance(getCenterX(), getCenterY(), destination_x, destination_y);

  const bool cantMove = !canMoveX && !canMoveY;
  const float deltaDistance = std::abs(last_distance - distanceToTarget);

  if (distanceToTarget < 10.0f || cantMove || deltaDistance < speed / 2.0f) {
    destination_x = static_cast<float>(asw::random::between(0, map_width));
    destination_y = static_cast<float>(asw::random::between(0, map_height));
    last_distance = 0.0f;
  } else {
    last_distance = distanceToTarget;
  }
}

void AiTank::ai_drive(const float deltaTime) {
  rotation_body = find_angle(position.x + 25, position.y + 25, destination_x,
                             destination_y);
  accelerate(true, deltaTime);
  drive(rotation_body, deltaTime);
}
