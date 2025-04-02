#include "AiTank.hpp"

#include "../system/ImageRegistry.hpp"

// Init
AiTank::AiTank(asw::scene::Scene<States>* scene,
               const asw::Vec2<float>& position,
               int health,
               int fireSpeed,
               int fireDelay,
               float speed,
               int team)
    : Tank(scene, position, health, fireSpeed, fireDelay, speed, team) {
  if (team == 1) {
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

  this->destination = position;
  transform.size = asw::util::getTextureSize(image_base);
}

// Update
void AiTank::update(float deltaTime) {
  Tank::update(deltaTime);
  find_enemy_target();
  update_target();
  ai_drive(deltaTime);
}

void AiTank::find_enemy_target() {
  // Rotate turret (at closest enemy)
  auto closest = asw::Vec2<float>(0, 0);
  bool found_enemy = false;
  const auto centre_tank = transform.getCenter();

  for (const auto& tank : scene->getObjectView<Tank>()) {
    if (tank->getTeam() == team) {
      continue;
    }

    found_enemy = true;
    const auto centre_enemy = tank->transform.getCenter();

    if (!found_enemy ||
        centre_tank.distance(centre_enemy) < centre_tank.distance(closest)) {
      closest = centre_enemy;
    }
  }

  if (found_enemy) {
    rotation_turret = transform.getCenter().angle(closest);

    // Shoot
    float distanceToEnemy = centre_tank.distance(closest);

    if (asw::random::between(0, 10) == 0 && distanceToEnemy < 500) {
      shoot(rotation_turret, transform.getCenter());
    }
  } else {
    rotation_turret = rotation_body;
  }
}

// Ai point choosing
void AiTank::update_target() {
  const float distanceToTarget = transform.getCenter().distance(destination);

  const bool cantMove = !canMoveX && !canMoveY;
  const float deltaDistance = std::abs(last_distance - distanceToTarget);

  if (distanceToTarget < 10.0f || cantMove || deltaDistance < speed / 2.0f) {
    destination.x = static_cast<float>(asw::random::between(0, map_width));
    destination.y = static_cast<float>(asw::random::between(0, map_height));
    last_distance = 0.0f;
  } else {
    last_distance = distanceToTarget;
  }
}

void AiTank::ai_drive(float deltaTime) {
  rotation_body = transform.getCenter().angle(destination);
  accelerate(true, deltaTime);
  drive(rotation_body, deltaTime);
}
