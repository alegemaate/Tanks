#include "PowerUp.hpp"

#include "../system/ImageRegistry.hpp"

PowerUp::PowerUp(float x, float y, PowerUpType type) : x(x), y(y), type(type) {
  switch (type) {
    case PowerUpType::HEALTH:
      imageKey = "power-up-health";
      break;
    case PowerUpType::SPEED:
      imageKey = "power-up-tank-speed";
      break;
    case PowerUpType::FIRE_SPEED:
      imageKey = "power-up-bullet-speed";
      break;
    case PowerUpType::FIRE_DELAY:
      imageKey = "power-up-bullet-delay";
      break;
    default:
      break;
  }
}

void PowerUp::draw(BITMAP* buffer) const {
  BITMAP* image = ImageRegistry::getImage(imageKey);
  draw_sprite(buffer, image, static_cast<int>(x), static_cast<int>(y));
}
