#pragma once

#include "../state/State.hpp"
#include "Tank.hpp"

class AiTank : public Tank {
 public:
  AiTank(asw::scene::Scene<States>* scene,
         const asw::Vec2<float>& position,
         int health,
         int fireSpeed,
         int fireDelay,
         float speed,
         int team);

  void update(float deltaTime) override;

 private:
  asw::Vec2<float> destination;
  float last_distance{0.0F};

  void update_target();
  void find_enemy_target();
  void ai_drive(float deltaTime);
};
