#ifndef SRC_GAME_PLAYER_TANK_H_
#define SRC_GAME_PLAYER_TANK_H_

#include "Tank.hpp"

class PlayerTank : public Tank {
 public:
  PlayerTank(asw::scene::Scene<States>* scene,
             asw::Camera& camera,
             const asw::Vec2<float>& position,
             int health,
             float fireSpeed,
             float fireDelay,
             float speed);

  void update(float dt) override;
};

#endif  // SRC_GAME_PLAYER_TANK_H_
