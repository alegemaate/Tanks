#ifndef SRC_GAME_PLAYER_TANK_H_
#define SRC_GAME_PLAYER_TANK_H_

#include "Tank.hpp"

class PlayerTank : public Tank {
 public:
  PlayerTank(World* world,
             float x,
             float y,
             int health,
             int fireSpeed,
             int fireDelay,
             float speed);

  void update(float deltaTime) override;
};

#endif  // SRC_GAME_PLAYER_TANK_H_
