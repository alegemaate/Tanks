#include "PlayerTank.hpp"

#include <vector>

#include "../system/ImageRegistry.hpp"

// Init
PlayerTank::PlayerTank(asw::scene::Scene<States>* scene, const asw::Vec2<float>& position,
    int health, int fireSpeed, int fireDelay, float speed)
    : Tank(scene, position, health, fireSpeed, fireDelay, speed, 0)
{
    image_treads = ImageRegistry::getImage("tank-treads");
    image_hurt = ImageRegistry::getImage("tank-dead");
    image_top = ImageRegistry::getImage("tank-turret-green");
    image_base = ImageRegistry::getImage("tank-base-green");

    transform.size = asw::util::get_texture_size(image_base);
}

// Update
void PlayerTank::update(float dt)
{
    using namespace asw::input;

    Tank::update(dt);

    // Screen size
    auto screenSize = asw::display::get_size();

    // Shoot
    rotation_turret = asw::Vec2<float>(
        static_cast<float>(screenSize.x) / 2.0F, static_cast<float>(screenSize.y) / 2.0F)
                          .angle(mouse.position);

    if (get_controller_axis(0, ControllerAxis::RightX) != 0
        || get_controller_axis(0, ControllerAxis::RightY) != 0) {
        // rotation_turret = find_angle(
        //     getCenterX() - 2.0F, getCenterY() - 2.0F,
        //     get_controller_axis(0, ControllerAxis::RightX) + (getCenterX()
        //     - 2.0F), get_controller_axis(0, ControllerAxis::RightY) +
        //     (getCenterY() - 2.0F));
    }

    if (get_key(Key::Space) || get_mouse_button(MouseButton::Left)
        || get_controller_axis(0, ControllerAxis::RightTrigger) != 0) {
        shoot(rotation_turret, transform.get_center());
    }

    // Rotate with keys
    if (get_key(Key::A) || get_key(Key::Left)) {
        rotation_body -= 0.03F * dt * 125.0F;
    }

    if (get_key(Key::D) || get_key(Key::Right)) {
        rotation_body += 0.03F * dt * 125.0F;
    }

    // Drive
    if (get_mouse_button(MouseButton::Right)) {
        rotation_body = rotation_turret;
    }

    if (get_controller_axis(0, ControllerAxis::LeftX) != 0
        || get_controller_axis(0, ControllerAxis::LeftY) != 0) {
        // rotation_body =
        //     find_angle(getCenterX(), getCenterY(),
        //                get_controller_axis(0, ControllerAxis::LeftX) +
        //                getCenterX(), get_controller_axis(0, ControllerAxis::LeftY)
        //                + getCenterY());
    }

    drive(rotation_body, dt);

    auto moving = get_mouse_button(MouseButton::Right)
        || get_controller_axis(0, ControllerAxis::LeftX) != 0
        || get_controller_axis(0, ControllerAxis::LeftY) != 0 || get_key(Key::W)
        || get_key(Key::Up);

    accelerate(moving, dt);
}
