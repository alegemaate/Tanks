#include "PlayerTank.hpp"

#include <vector>

#include "../system/ImageRegistry.hpp"

// Init
PlayerTank::PlayerTank(World* world,
                       float x,
                       float y,
                       int health,
                       int fireSpeed,
                       int fireDelay,
                       float speed)
    : Tank(world, x, y, health, fireSpeed, fireDelay, speed) {
  this->image_treads = ImageRegistry::getImage("tank-treads");
  this->image_hurt = ImageRegistry::getImage("tank-dead");
  this->image_top = ImageRegistry::getImage("tank-turret-green");
  this->image_base = ImageRegistry::getImage("tank-base-green");

  auto imageSize = asw::util::getTextureSize(image_base);
  this->width = static_cast<float>(imageSize.x);
  this->height = static_cast<float>(imageSize.y);
}

// Update
void PlayerTank::update(float deltaTime) {
  using namespace asw::input;

  Tank::update(deltaTime);

  if (dead) {
    return;
  }

  // Screen size
  auto screenSize = asw::display::getSize();

  // Shoot
  rotation_turret =
      find_angle(static_cast<float>(screenSize.x) / 2.0f,
                 static_cast<float>(screenSize.y) / 2.0f,
                 static_cast<float>(mouse.x), static_cast<float>(mouse.y));

  if (getControllerAxis(0, ControllerAxis::RIGHT_X) != 0 ||
      getControllerAxis(0, ControllerAxis::RIGHT_Y) != 0) {
    rotation_turret = find_angle(
        getCenterX() - 2.0f, getCenterY() - 2.0f,
        getControllerAxis(0, ControllerAxis::RIGHT_X) + (getCenterX() - 2.0f),
        getControllerAxis(0, ControllerAxis::RIGHT_Y) + (getCenterY() - 2.0f));
  }

  if (isKeyDown(Key::SPACE) || isButtonDown(MouseButton::LEFT) ||
      getControllerAxis(0, ControllerAxis::RIGHT_TRIGGER) != 0) {
    shoot(rotation_turret, getCenterX() - 2, getCenterY() - 2);
  }

  // Rotate with keys
  if (isKeyDown(Key::A) || isKeyDown(Key::LEFT)) {
    rotation_body -= 0.03f * (deltaTime / 8.0f);
  }

  if (isKeyDown(Key::D) || isKeyDown(Key::RIGHT)) {
    rotation_body += 0.03f * (deltaTime / 8.0f);
  }

  // Drive
  if (isButtonDown(MouseButton::RIGHT)) {
    rotation_body = rotation_turret;
  }

  if (getControllerAxis(0, ControllerAxis::LEFT_X) != 0 ||
      getControllerAxis(0, ControllerAxis::LEFT_Y) != 0) {
    rotation_body =
        find_angle(getCenterX(), getCenterY(),
                   getControllerAxis(0, ControllerAxis::LEFT_X) + getCenterX(),
                   getControllerAxis(0, ControllerAxis::LEFT_Y) + getCenterY());
  }

  drive(rotation_body, deltaTime);

  auto moving = isButtonDown(MouseButton::RIGHT) ||
                getControllerAxis(0, ControllerAxis::LEFT_X) != 0 ||
                getControllerAxis(0, ControllerAxis::LEFT_Y) != 0 ||
                isKeyDown(Key::W) || isKeyDown(Key::UP);

  accelerate(moving, deltaTime);
}

// Feed AI player positions
void Tank::process_enemies(std::vector<std::shared_ptr<Tank>>* tempOtherTanks) {
  otherTanks = tempOtherTanks;
}
