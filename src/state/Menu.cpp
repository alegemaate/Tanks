#include "Menu.hpp"

#include <algorithm>

constexpr float BUTTON_PADDING = 10.0F;

namespace {
// Step a map size, without going below the smallest map or wrapping
void stepMapSize(unsigned char& size, int step) {
  size = static_cast<unsigned char>(
      std::clamp(size + step, static_cast<int>(Game::MIN_MAP_SIZE), 255));
}
}  // namespace

// Initializer
void Menu::init() {
  // Entity
  auto background = create_object<asw::game::Sprite>();
  background->set_texture(asw::assets::get_texture("menu-background"));

  // Font
  font = asw::assets::get_font("main");

  // UI
  ui.root.clear_children();
  ui.ctx.theme.font = font;
  ui.ctx.theme.button.bg = asw::Color(200, 200, 200);
  ui.ctx.theme.button.bg_hover = asw::Color(220, 220, 220);
  ui.ctx.theme.button.bg_pressed = asw::Color(240, 240, 240);
  ui.ctx.theme.button.text = asw::color::black;
  ui.ctx.theme.button.text_hover = asw::color::black;
  ui.ctx.theme.button.border = asw::color::black;
  ui.ctx.theme.button.border_width = 1.0F;
  ui.ctx.navigation = asw::ui::bind_default_navigation();

  // Make teams
  addStepper(90, [](int step) { Game::num_friends += step; });
  addStepper(210, [](int step) { Game::num_enemies += step; });
  addStepper(330, [](int step) { stepMapSize(Game::map_width, step); });
  addStepper(450, [](int step) { stepMapSize(Game::map_height, step); });
  addStepper(570, [](int step) { Tank::num_bullet_bounces += step; });

  // Start game
  auto& start = addButton(340, 485, "START");
  start.on_click = [this]() { manager.set_next_scene(States::Game); };
  ui.focus(start);
}

asw::ui::Button& Menu::addButton(float x, float y, const std::string& text) {
  auto& button = ui.root.add_child<asw::ui::Button>();
  button.padding = BUTTON_PADDING;
  button.set_text(text, true);
  button.transform.set_position(x, y);
  return button;
}

void Menu::addStepper(float x, const std::function<void(int)>& change) {
  addButton(x, 275, "/\\").on_click = [change]() { change(1); };
  addButton(x, 335, "\\/").on_click = [change]() { change(-1); };
}

// Update routine
void Menu::update(float dt) {
  Scene::update(dt);
  ui.update();
}

// Drawing routine
void Menu::draw() {
  Scene::draw();
  ui.draw();

  // Player nums
  const auto drawNumber = [this](int value, float x) {
    asw::draw::text(font, std::to_string(value), asw::Vec2<float>(x, 315),
                    asw::color::black, asw::TextJustify::Center);
  };

  drawNumber(Game::num_friends, 109);
  drawNumber(Game::num_enemies, 229);
  drawNumber(Game::map_width, 349);
  drawNumber(Game::map_height, 469);
  drawNumber(Tank::num_bullet_bounces, 589);
}
