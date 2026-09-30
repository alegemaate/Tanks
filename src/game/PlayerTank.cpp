#include "PlayerTank.hpp"

// Turn speed with keys (radians per second)
constexpr float TURN_SPEED = 3.75F;

// Init
PlayerTank::PlayerTank(asw::scene::Scene<States>* scene,
                       asw::Camera& camera,
                       const asw::Vec2<float>& position,
                       int health,
                       float fireSpeed,
                       float fireDelay,
                       float speed)
    : Tank(scene,
           camera,
           position,
           health,
           fireSpeed,
           fireDelay,
           speed,
           0) {
  image_treads = asw::assets::get_texture("tank-treads");
  image_hurt = asw::assets::get_texture("tank-dead");
  image_top = asw::assets::get_texture("tank-turret-green");
  image_base = asw::assets::get_texture("tank-base-green");

  transform.size = asw::util::get_texture_size(image_base);
}

// Update
void PlayerTank::update(float dt) {
  using namespace asw::input;

  Tank::update(dt);

  const auto center = transform.get_center();

  // Aim with the right stick, or else the mouse
  const auto aim_stick =
      get_controller_stick(ANY_CONTROLLER, ControllerStick::Right);

  if (aim_stick.magnitude() > 0.0F) {
    rotation_turret = center.angle(center + aim_stick);
  } else {
    rotation_turret =
        center.angle(camera.screen_to_world(get_mouse().position));
  }

  if (get_action("fire")) {
    shoot(rotation_turret, center);
  }

  // Rotate with keys
  if (get_action("turn_left")) {
    rotation_body -= TURN_SPEED * dt;
  }

  if (get_action("turn_right")) {
    rotation_body += TURN_SPEED * dt;
  }

  // Drive towards the turret with the mouse, or the left stick
  const auto drive_stick =
      get_controller_stick(ANY_CONTROLLER, ControllerStick::Left);

  if (get_mouse_button(MouseButton::Right)) {
    rotation_body = rotation_turret;
  } else if (drive_stick.magnitude() > 0.0F) {
    rotation_body = center.angle(center + drive_stick);
  }

  drive(rotation_body, dt);

  accelerate(get_action("drive") || drive_stick.magnitude() > 0.0F, dt);
}
