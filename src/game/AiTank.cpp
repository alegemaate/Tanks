#include "AiTank.hpp"

// Init
AiTank::AiTank(asw::scene::Scene<States>* scene,
               asw::Camera& camera,
               const asw::Vec2<float>& position,
               int health,
               float fireSpeed,
               float fireDelay,
               float speed,
               int team)
    : Tank(scene,
           camera,
           position,
           health,
           fireSpeed,
           fireDelay,
           speed,
           team) {
  if (team == 1) {
    this->image_treads = asw::assets::get_texture("tank-treads");
    this->image_hurt = asw::assets::get_texture("tank-dead");
    this->image_top = asw::assets::get_texture("tank-turret-red");
    this->image_base = asw::assets::get_texture("tank-base-red");
  } else {
    this->image_treads = asw::assets::get_texture("tank-treads");
    this->image_hurt = asw::assets::get_texture("tank-dead");
    this->image_top = asw::assets::get_texture("tank-turret-blue");
    this->image_base = asw::assets::get_texture("tank-base-blue");
  }

  this->destination = position;
  transform.size = asw::util::get_texture_size(image_base);
}

// Update
void AiTank::update(float dt) {
  Tank::update(dt);
  find_enemy_target();
  update_target(dt);
  ai_drive(dt);
}

void AiTank::find_enemy_target() {
  // Rotate turret (at closest enemy)
  auto closest = asw::Vec2<float>(0, 0);
  bool found_enemy = false;
  const auto centre_tank = transform.get_center();

  for (const auto& tank : scene->get_object_view<Tank>()) {
    if (tank->getTeam() == team) {
      continue;
    }

    found_enemy = true;
    const auto centre_enemy = tank->transform.get_center();

    if (!found_enemy ||
        centre_tank.distance(centre_enemy) < centre_tank.distance(closest)) {
      closest = centre_enemy;
    }
  }

  if (found_enemy) {
    rotation_turret = transform.get_center().angle(closest);

    // Shoot
    float distanceToEnemy = centre_tank.distance(closest);

    if (asw::random::between(0, 10) == 0 && distanceToEnemy < 500) {
      shoot(rotation_turret, transform.get_center());
    }
  } else {
    rotation_turret = rotation_body;
  }
}

// Ai point choosing
void AiTank::update_target(float dt) {
  const float distanceToTarget = transform.get_center().distance(destination);

  const bool cantMove = !canMoveX && !canMoveY;
  const float deltaDistance = std::abs(last_distance - distanceToTarget);

  if (distanceToTarget < 10.0f || cantMove || deltaDistance < speed * dt / 2.0F) {
    destination.x = static_cast<float>(asw::random::between(0, map_width));
    destination.y = static_cast<float>(asw::random::between(0, map_height));
    last_distance = 0.0f;
  } else {
    last_distance = distanceToTarget;
  }
}

void AiTank::ai_drive(float dt) {
  rotation_body = transform.get_center().angle(destination);
  accelerate(true, dt);
  drive(rotation_body, dt);
}
