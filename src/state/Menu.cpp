#include "Menu.hpp"

#include "../components/Sprite.hpp"
#include "../components/Transform.hpp"
#include "../system/ImageRegistry.hpp"

// Initializer
void Menu::init() {
  // Entity
  const auto background = m_registry.create();
  m_registry.emplace<Sprite>(background, "menu-background");
  m_registry.emplace<Transform>(background, 0.0, 0.0);

  // Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 12);

  // Buttons
  friends_up = Button(90, 275, "/\\", font);
  friends_down = Button(90, 335, "\\/", font);
  enemies_up = Button(210, 275, "/\\", font);
  enemies_down = Button(210, 335, "\\/", font);
  width_up = Button(330, 275, "/\\", font);
  width_down = Button(330, 335, "\\/", font);
  height_up = Button(450, 275, "/\\", font);
  height_down = Button(450, 335, "\\/", font);
  bounce_up = Button(570, 275, "/\\", font);
  bounce_down = Button(570, 335, "\\/", font);
  start = Button(340, 485, "START", font);
}

// Update routine
void Menu::update(float deltaTime) {
  // Update buttons
  enemies_up.update();
  enemies_down.update();
  friends_up.update();
  friends_down.update();
  width_up.update();
  width_down.update();
  height_up.update();
  height_down.update();
  bounce_up.update();
  bounce_down.update();
  start.update();

  // Make teams
  if (enemies_up.clicked()) {
    Game::num_enemies++;
  }
  if (enemies_down.clicked()) {
    Game::num_enemies--;
  }
  if (friends_up.clicked()) {
    Game::num_friends++;
  }
  if (friends_down.clicked()) {
    Game::num_friends--;
  }
  if (width_up.clicked()) {
    Game::map_width++;
  }
  if (width_down.clicked()) {
    Game::map_width--;
  }
  if (height_up.clicked()) {
    Game::map_height++;
  }
  if (height_down.clicked()) {
    Game::map_height--;
  }
  if (bounce_up.clicked()) {
    Tank::num_bullet_bounces++;
  }
  if (bounce_down.clicked()) {
    Tank::num_bullet_bounces--;
  }

  // Start game
  if (start.clicked()) {
    sceneManager.setNextScene(States::Game);
  }
}

// Drawing routine
void Menu::draw() {
  // Render system
  m_render_system.render(m_registry);

  // Buttons
  enemies_up.draw();
  enemies_down.draw();
  friends_up.draw();
  friends_down.draw();
  width_up.draw();
  width_down.draw();
  height_up.draw();
  height_down.draw();
  bounce_up.draw();
  bounce_down.draw();
  start.draw();

  // Player nums
  asw::draw::textCenter(font, std::to_string(Game::num_friends),
                        asw::Vec2<float>(109, 315),
                        asw::util::makeColor(0, 0, 0));
  asw::draw::textCenter(font, std::to_string(Game::num_enemies),
                        asw::Vec2<float>(229, 315),
                        asw::util::makeColor(0, 0, 0));
  asw::draw::textCenter(font, std::to_string(Game::map_width),
                        asw::Vec2<float>(349, 315),
                        asw::util::makeColor(0, 0, 0));
  asw::draw::textCenter(font, std::to_string(Game::map_height),
                        asw::Vec2<float>(469, 315),
                        asw::util::makeColor(0, 0, 0));
  asw::draw::textCenter(font, std::to_string(Tank::num_bullet_bounces),
                        asw::Vec2<float>(589, 315),
                        asw::util::makeColor(0, 0, 0));

  // Mouse
  asw::draw::rectFill(
      asw::Quad<float>(asw::input::mouse.x, asw::input::mouse.y, 4, 4),
      asw::util::makeColor(255, 255, 255));
}
