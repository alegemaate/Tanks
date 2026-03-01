#include "Menu.hpp"

#include "../components/Sprite.hpp"
#include "../components/Transform.hpp"
#include "../system/ImageRegistry.hpp"

// Initializer
void Menu::init()
{
    // Font
    font = asw::assets::load_font("assets/fonts/ariblk.ttf", 12);

    // UI Root
    ui_root = asw::ui::Root();
    ui_root.root.bg_image = ImageRegistry::getImage("menu-background");
    ui_root.root.transform.set_size(800, 600);

    // Buttons
    auto& friends_up = ui_root.root.add_child<asw::ui::Button>();
    friends_up.font = font;
    friends_up.transform.set_position(90, 275);
    friends_up.transform.set_size(40, 40);
    friends_up.text = "/\\";
    friends_up.on_click = []() { Game::num_friends++; };

    auto& friends_down = ui_root.root.add_child<asw::ui::Button>();
    friends_down.font = font;
    friends_down.transform.set_position(90, 335);
    friends_down.transform.set_size(40, 40);
    friends_down.text = "\\/";
    friends_down.on_click = []() { Game::num_friends--; };

    auto& enemies_up = ui_root.root.add_child<asw::ui::Button>();
    enemies_up.font = font;
    enemies_up.transform.set_position(210, 275);
    enemies_up.transform.set_size(40, 40);
    enemies_up.text = "/\\";
    enemies_up.on_click = []() { Game::num_enemies++; };

    auto& enemies_down = ui_root.root.add_child<asw::ui::Button>();
    enemies_down.font = font;
    enemies_down.transform.set_position(210, 335);
    enemies_down.transform.set_size(40, 40);
    enemies_down.text = "\\/";
    enemies_down.on_click = []() { Game::num_enemies--; };

    auto& width_up = ui_root.root.add_child<asw::ui::Button>();
    width_up.font = font;
    width_up.transform.set_position(330, 275);
    width_up.transform.set_size(40, 40);
    width_up.text = "/\\";
    width_up.on_click = []() { Game::map_width++; };

    auto& width_down = ui_root.root.add_child<asw::ui::Button>();
    width_down.font = font;
    width_down.transform.set_position(330, 335);
    width_down.transform.set_size(40, 40);
    width_down.text = "\\/";
    width_down.on_click = []() { Game::map_width--; };

    auto& height_up = ui_root.root.add_child<asw::ui::Button>();
    height_up.font = font;
    height_up.transform.set_position(450, 275);
    height_up.transform.set_size(40, 40);
    height_up.text = "/\\";
    height_up.on_click = []() { Game::map_height++; };

    auto& height_down = ui_root.root.add_child<asw::ui::Button>();
    height_down.font = font;
    height_down.transform.set_position(450, 335);
    height_down.transform.set_size(40, 40);
    height_down.text = "\\/";
    height_down.on_click = []() { Game::map_height--; };

    auto& bounce_up = ui_root.root.add_child<asw::ui::Button>();
    bounce_up.font = font;
    bounce_up.transform.set_position(570, 275);
    bounce_up.transform.set_size(40, 40);
    bounce_up.text = "/\\";
    bounce_up.on_click = []() { Tank::num_bullet_bounces++; };

    auto& bounce_down = ui_root.root.add_child<asw::ui::Button>();
    bounce_down.font = font;
    bounce_down.transform.set_position(570, 335);
    bounce_down.transform.set_size(40, 40);
    bounce_down.text = "\\/";
    bounce_down.on_click = []() { Tank::num_bullet_bounces--; };

    auto& start = ui_root.root.add_child<asw::ui::Button>();
    start.font = font;
    start.transform.set_position(340, 485);
    start.transform.set_size(120, 40);
    start.text = "START";
    start.on_click = [this]() { manager.set_next_scene(States::Game); };
}

// Update routine
void Menu::update(float dt)
{
    Scene::update(dt);
    ui_root.update();
}

// Drawing routine
void Menu::draw()
{
    Scene::draw();

    ui_root.draw();

    // Player nums
    asw::draw::text(font, std::to_string(Game::num_friends), asw::Vec2<float>(109, 315),
        asw::Color(0, 0, 0), asw::TextJustify::Center);
    asw::draw::text(font, std::to_string(Game::num_enemies), asw::Vec2<float>(229, 315),
        asw::Color(0, 0, 0), asw::TextJustify::Center);
    asw::draw::text(font, std::to_string(Game::map_width), asw::Vec2<float>(349, 315),
        asw::Color(0, 0, 0), asw::TextJustify::Center);
    asw::draw::text(font, std::to_string(Game::map_height), asw::Vec2<float>(469, 315),
        asw::Color(0, 0, 0), asw::TextJustify::Center);
    asw::draw::text(font, std::to_string(Tank::num_bullet_bounces), asw::Vec2<float>(589, 315),
        asw::Color(0, 0, 0), asw::TextJustify::Center);
}
