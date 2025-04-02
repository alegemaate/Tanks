#pragma once

#include <asw/asw.h>
#include <string>

enum class PowerUpType { HEALTH, SPEED, FIRE_SPEED, FIRE_DELAY };

class PowerUp : public asw::game::GameObject {
 public:
  PowerUp(float x, float y, PowerUpType type);

  PowerUpType getType() const { return type; }

  void pickup() { alive = false; }

  void draw() override;

 private:
  PowerUpType type;

  std::string imageKey;
};
