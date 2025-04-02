#include "PlayerTank.hpp"

#include <vector>

#include "../system/ImageRegistry.hpp"

// Init
PlayerTank::PlayerTank(asw::scene::Scene<States>* scene,
                       const asw::Vec2<float>& position,
                       int health,
                       int fireSpeed,
                       int fireDelay,
                       float speed)
    : Tank(scene, position, health, fireSpeed, fireDelay, speed, 0) {
  image_treads = ImageRegistry::getImage("tank-treads");
  image_hurt = ImageRegistry::getImage("tank-dead");
  image_top = ImageRegistry::getImage("tank-turret-green");
  image_base = ImageRegistry::getImage("tank-base-green");

  transform.size = asw::util::getTextureSize(image_base);
}

// Update
void PlayerTank::update(float deltaTime) {
  using namespace asw::input;

  Tank::update(deltaTime);

  // Screen size
  auto screenSize = asw::display::getSize();

  // Shoot
  rotation_turret = asw::Vec2<float>(static_cast<float>(screenSize.x) / 2.0F,
                                     static_cast<float>(screenSize.y) / 2.0F)
                        .angle(asw::Vec2<float>(mouse.x, mouse.y));

  if (getControllerAxis(0, ControllerAxis::RIGHT_X) != 0 ||
      getControllerAxis(0, ControllerAxis::RIGHT_Y) != 0) {
    // rotation_turret = find_angle(
    //     getCenterX() - 2.0F, getCenterY() - 2.0F,
    //     getControllerAxis(0, ControllerAxis::RIGHT_X) + (getCenterX()
    //     - 2.0F), getControllerAxis(0, ControllerAxis::RIGHT_Y) +
    //     (getCenterY() - 2.0F));
  }

  if (isKeyDown(Key::SPACE) || isButtonDown(MouseButton::LEFT) ||
      getControllerAxis(0, ControllerAxis::RIGHT_TRIGGER) != 0) {
    shoot(rotation_turret, transform.getCenter());
  }

  // Rotate with keys
  if (isKeyDown(Key::A) || isKeyDown(Key::LEFT)) {
    rotation_body -= 0.03F * (deltaTime / 8.0F);
  }

  if (isKeyDown(Key::D) || isKeyDown(Key::RIGHT)) {
    rotation_body += 0.03F * (deltaTime / 8.0F);
  }

  // Drive
  if (isButtonDown(MouseButton::RIGHT)) {
    rotation_body = rotation_turret;
  }

  if (getControllerAxis(0, ControllerAxis::LEFT_X) != 0 ||
      getControllerAxis(0, ControllerAxis::LEFT_Y) != 0) {
    // rotation_body =
    //     find_angle(getCenterX(), getCenterY(),
    //                getControllerAxis(0, ControllerAxis::LEFT_X) +
    //                getCenterX(), getControllerAxis(0, ControllerAxis::LEFT_Y)
    //                + getCenterY());
  }

  drive(rotation_body, deltaTime);

  auto moving = isButtonDown(MouseButton::RIGHT) ||
                getControllerAxis(0, ControllerAxis::LEFT_X) != 0 ||
                getControllerAxis(0, ControllerAxis::LEFT_Y) != 0 ||
                isKeyDown(Key::W) || isKeyDown(Key::UP);

  accelerate(moving, deltaTime);
}
