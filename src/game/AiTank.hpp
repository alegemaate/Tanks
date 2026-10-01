#pragma once

#include "../state/State.hpp"
#include "Tank.hpp"

class AiTank : public Tank {
 public:
  AiTank(asw::scene::Scene<States>* scene,
         asw::Camera& camera,
         const asw::Vec2<float>& position,
         int health,
         float fireSpeed,
         float fireDelay,
         float speed,
         int team);

  void update(float dt) override;

 private:
  asw::Vec2<float> destination;
  float last_distance{0.0F};

  void find_enemy_target();
  void update_target(float dt);
  void ai_drive(float dt);
};
