#include "Menu.hpp"

#include "../components/Sprite.hpp"
#include "../components/Transform.hpp"
#include "../system/ImageRegistry.hpp"

// Initializer
void Menu::init() {
  // Entity
  auto background = createObject<asw::game::Sprite>();
  background->setTexture(ImageRegistry::getImage("menu-background"));

  // Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 12);

  // Buttons
  friends_up = createObject<Button>(90, 275, "/\\", font);
  friends_down = createObject<Button>(90, 335, "\\/", font);
  enemies_up = createObject<Button>(210, 275, "/\\", font);
  enemies_down = createObject<Button>(210, 335, "\\/", font);
  width_up = createObject<Button>(330, 275, "/\\", font);
  width_down = createObject<Button>(330, 335, "\\/", font);
  height_up = createObject<Button>(450, 275, "/\\", font);
  height_down = createObject<Button>(450, 335, "\\/", font);
  bounce_up = createObject<Button>(570, 275, "/\\", font);
  bounce_down = createObject<Button>(570, 335, "\\/", font);
  start = createObject<Button>(340, 485, "START", font);
}

// Update routine
void Menu::update(float deltaTime) {
  Scene::update(deltaTime);

  // Make teams
  if (enemies_up->clicked()) {
    Game::num_enemies++;
  }
  if (enemies_down->clicked()) {
    Game::num_enemies--;
  }
  if (friends_up->clicked()) {
    Game::num_friends++;
  }
  if (friends_down->clicked()) {
    Game::num_friends--;
  }
  if (width_up->clicked()) {
    Game::map_width++;
  }
  if (width_down->clicked()) {
    Game::map_width--;
  }
  if (height_up->clicked()) {
    Game::map_height++;
  }
  if (height_down->clicked()) {
    Game::map_height--;
  }
  if (bounce_up->clicked()) {
    Tank::num_bullet_bounces++;
  }
  if (bounce_down->clicked()) {
    Tank::num_bullet_bounces--;
  }

  // Start game
  if (start->clicked()) {
    sceneManager.setNextScene(States::Game);
  }
}

// Drawing routine
void Menu::draw() {
  Scene::draw();

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
}
