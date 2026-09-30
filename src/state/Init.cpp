#include "Init.hpp"

void Init::init() {
  asw::log::info("Setting up");

  // Window Title
  asw::display::set_title("Tanks!");

  // Load images
  asw::assets::load_texture("assets/images/menu.png", "menu-background");
  asw::assets::load_texture("assets/images/background.png", "game-background");
  asw::assets::load_texture("assets/images/cursor.png", "cursor");
  asw::assets::load_texture("assets/images/block_box_1.png", "block-box");
  asw::assets::load_texture("assets/images/block_stone_1.png", "block-stone");
  asw::assets::load_texture("assets/images/powerup_health.png",
                            "power-up-health");
  asw::assets::load_texture("assets/images/powerup_tank_speed.png",
                            "power-up-tank-speed");
  asw::assets::load_texture("assets/images/powerup_bullet_speed.png",
                            "power-up-bullet-speed");
  asw::assets::load_texture("assets/images/powerup_bullet_delay.png",
                            "power-up-bullet-delay");
  asw::assets::load_texture("assets/images/tank_treads.png", "tank-treads");
  asw::assets::load_texture("assets/images/tank_dead.png", "tank-dead");
  asw::assets::load_texture("assets/images/tank_turret_green.png",
                            "tank-turret-green");
  asw::assets::load_texture("assets/images/tank_base_green.png",
                            "tank-base-green");
  asw::assets::load_texture("assets/images/tank_turret_red.png",
                            "tank-turret-red");
  asw::assets::load_texture("assets/images/tank_base_red.png",
                            "tank-base-red");
  asw::assets::load_texture("assets/images/tank_turret_blue.png",
                            "tank-turret-blue");
  asw::assets::load_texture("assets/images/tank_base_blue.png",
                            "tank-base-blue");
  asw::assets::load_texture("assets/images/test.png", "light");

  asw::assets::load_sample("assets/sfx/explode.wav", "explode");
  asw::assets::load_sample("assets/sfx/fire.wav", "fire");
  asw::assets::load_sample("assets/sfx/tank_explode.wav", "tank-explode");

  asw::assets::load_font("assets/fonts/ariblk.ttf", 12, "main");

  // Player controls
  using namespace asw::input;
  bind_action("fire", KeyBinding{Key::Space});
  bind_action("fire", MouseButtonBinding{MouseButton::Left});
  bind_action("fire", ControllerAxisBinding{ControllerAxis::RightTrigger,
                                            ANY_CONTROLLER});
  bind_action("drive", KeyBinding{Key::W});
  bind_action("drive", KeyBinding{Key::Up});
  bind_action("drive", MouseButtonBinding{MouseButton::Right});
  bind_action("turn_left", KeyBinding{Key::A});
  bind_action("turn_left", KeyBinding{Key::Left});
  bind_action("turn_right", KeyBinding{Key::D});
  bind_action("turn_right", KeyBinding{Key::Right});
  bind_action("menu", KeyBinding{Key::M});
  bind_action("menu", ControllerButtonBinding{ControllerButton::Back,
                                              ANY_CONTROLLER});

  asw::log::info("Loaded assets");
}
