#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <asw/asw.h>
#include <string>

enum class PowerUpType { HEALTH, SPEED, FIRE_SPEED, FIRE_DELAY };

class PowerUp {
 public:
  PowerUp(float x, float y, PowerUpType type);

  float getX() const { return position.x; }
  float getY() const { return position.y; }
  static float getWidth() { return 40.0F; }
  static float getHeight() { return 40.0F; }
  PowerUpType getType() const { return type; }

  bool getDead() const { return dead; }

  void pickup() { dead = true; }

  void draw() const;

 private:
  asw::Vec2<float> position;

  PowerUpType type;

  bool dead = false;

  std::string imageKey;
};

#endif  // SRC_GAME_POWERUP_H_
