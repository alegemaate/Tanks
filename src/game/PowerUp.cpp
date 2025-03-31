#include "PowerUp.hpp"

#include "../system/ImageRegistry.hpp"

PowerUp::PowerUp(float x, float y, PowerUpType type)
    : position(x, y), type(type) {
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

void PowerUp::draw() const {
  asw::Texture image = ImageRegistry::getImage(imageKey);
  asw::draw::sprite(image, position);
}
