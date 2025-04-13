#include "PowerUp.hpp"

#include "../system/ImageRegistry.hpp"

PowerUp::PowerUp(float x, float y, PowerUpType type) : type(type) {
  transform = asw::Quad<float>(x, y, 40.0F, 40.0F);

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

  zIndex = 6;
}

void PowerUp::draw() {
  asw::Texture image = ImageRegistry::getImage(imageKey);
  asw::draw::sprite(image, transform.position);
}
