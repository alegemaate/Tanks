#ifndef SRC_GAME_PLAYER_TANK_H_
#define SRC_GAME_PLAYER_TANK_H_

#include "Tank.hpp"

class PlayerTank : public Tank {
 public:
  PlayerTank(asw::scene::Scene<States>* scene,
             const asw::Vec2<float>& position,
             int health,
             int fireSpeed,
             int fireDelay,
             float speed);

  void update(float deltaTime) override;
};

#endif  // SRC_GAME_PLAYER_TANK_H_
