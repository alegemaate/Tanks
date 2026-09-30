#include "Game.hpp"

#include <algorithm>

unsigned char Game::map_width = 20;
unsigned char Game::map_height = 20;

unsigned char Game::num_enemies = 5;
unsigned char Game::num_friends = 5;

const unsigned char max_map_width = 255;
const unsigned char max_map_height = 255;

// Seconds a won or lost round stays on screen
constexpr float ROUND_END_DELAY = 3.0F;

// Init state (and Game)
void Game::init() {
  const int map_pixel_width = map_width * 40;
  const int map_pixel_height = map_height * 40;

  // Create buffers
  decal_buffer = asw::assets::create_texture(map_pixel_width, map_pixel_height);
  map_buffer = asw::assets::create_texture(map_pixel_width, map_pixel_height);
  light_buffer = asw::assets::create_texture(map_pixel_width, map_pixel_height);
  fade_buffer = asw::assets::create_texture(map_pixel_width, map_pixel_height);

  // Setup light buffer
  asw::draw::set_blend_mode(light_buffer, asw::BlendMode::Add);
  asw::draw::set_alpha(light_buffer, 0.5F);

  // Setup fade buffer
  asw::display::set_render_target(fade_buffer);
  asw::draw::clear_color(asw::Color(0, 0, 0, 10));
  asw::display::reset_render_target();
  asw::draw::set_blend_mode(fade_buffer, asw::BlendMode::Blend);

  // Load images
  background = asw::assets::get_texture("game-background");
  cursor = asw::assets::get_texture("cursor");

  // Font
  font = asw::assets::get_font("main");

  // Camera
  const auto screen_size = asw::display::get_logical_size();
  camera.set_view_size(asw::Vec2<float>(screen_size.x, screen_size.y));
  camera.set_bounds(asw::Quad<float>(0, 0, map_pixel_width, map_pixel_height));

  currentRound = 0;
  startRound();
}

void Game::startRound() {
  // Remove the last round's objects
  Scene::cleanup();

  currentRound++;
  roundState = RoundState::Playing;
  timer = 0.0F;

  // Clear decals
  asw::display::set_render_target(decal_buffer);
  asw::draw::clear_color(asw::Color(0, 0, 0, 0));

  // Clear lights to the dark they fade to
  asw::display::set_render_target(light_buffer);
  asw::draw::clear_color(asw::Color(0, 0, 0, 255));
  asw::display::reset_render_target();

  generateMap();
}

void Game::updateRoundState() {
  friendsLeft = 0;
  enemiesLeft = 0;
  bool playerAlive = false;

  for (const auto& tank : get_object_view<Tank>()) {
    if (tank->isDead()) {
      continue;
    }

    if (tank->getTeam() == 0) {
      friendsLeft++;
    } else {
      enemiesLeft++;
    }
  }

  for (const auto& tank : get_object_view<PlayerTank>()) {
    playerAlive = !tank->isDead();
  }

  if (!playerAlive) {
    roundState = RoundState::Lost;
  } else if (enemiesLeft == 0) {
    roundState = RoundState::Won;
  }
}

void Game::update(float dt) {
  // Update world
  Scene::update(dt);

  // Return to menu
  if (asw::input::get_action_down("menu")) {
    manager.set_next_scene(States::Menu);
  }

  // Win or lose
  if (roundState == RoundState::Playing) {
    updateRoundState();
  } else {
    timer += dt;

    if (timer >= ROUND_END_DELAY) {
      if (roundState == RoundState::Won) {
        startRound();
      } else {
        manager.set_next_scene(States::Menu);
      }
      return;
    }
  }

  // Follow player
  for (const auto& tank : get_object_view<PlayerTank>()) {
    const auto center = tank->transform.get_center();
    camera.follow(center, dt);
    asw::sound::set_listener(center);
  }

  camera.update(dt);
}

void Game::draw() {
  // Draw background
  asw::draw::sprite(background, asw::Vec2<float>(0, 0));

  // Draw decals
  asw::display::set_render_target(decal_buffer);
  for (auto& tank : get_object_view<Tank>()) {
    tank->putDecal();
  }

  // Draw lights
  asw::display::set_render_target(light_buffer);
  asw::draw::set_blend_mode(light_buffer, asw::BlendMode::Add);
  asw::draw::sprite(fade_buffer, asw::Vec2<float>(0, 0));
  for (auto& bullet : get_object_view<Bullet>()) {
    bullet->drawLight();
  }

  for (auto& bullet : get_object_view<Particle>()) {
    bullet->drawLight();
  }

  // Blank map map_buffer
  asw::display::set_render_target(map_buffer);
  asw::draw::clear_color(asw::Color(0, 88, 0));

  // Decal to buffer
  asw::draw::sprite(decal_buffer, asw::Vec2<float>(0, 0));

  // Draw world
  Scene::draw();

  // Light to buffer
  asw::draw::set_blend_mode(light_buffer, asw::BlendMode::Multiply);
  asw::draw::sprite(light_buffer, asw::Vec2<float>(0, 0));

  // Map to buffer
  asw::display::reset_render_target();
  asw::draw::sprite(map_buffer, camera.world_to_screen(asw::Vec2<float>(0, 0)));

  // Text
  asw::draw::text_shadow(font, "Round: " + std::to_string(currentRound),
                         asw::Vec2<float>(20, 20), asw::color::white);
  asw::draw::text_shadow(font, "Team BLUE: " + std::to_string(friendsLeft),
                         asw::Vec2<float>(20, 40), asw::color::white);
  asw::draw::text_shadow(font, "Team RED: " + std::to_string(enemiesLeft),
                         asw::Vec2<float>(20, 60), asw::color::white);

  if (roundState != RoundState::Playing) {
    const auto screen_size = asw::display::get_logical_size();
    const auto message =
        roundState == RoundState::Won
            ? "Round " + std::to_string(currentRound) + " cleared!"
            : "Game over";

    asw::draw::text_shadow(
        font, message,
        asw::Vec2<float>(screen_size.x / 2.0F, screen_size.y / 2.0F),
        asw::color::white, asw::color::black, asw::Vec2<float>(2.0F, 2.0F),
        asw::TextJustify::Center);
  }

  // Cursor
  asw::draw::sprite(
      cursor, asw::input::get_mouse().position - asw::Vec2<float>(10, 10));
}

void Game::generateMap() {
  // Make a map
  std::array<std::array<BarrierType, max_map_height>, max_map_width> map_temp{};
  std::vector<asw::Vec2<float>> startLocations{};

  for (int pass = 0; pass < 8; pass++) {
    for (unsigned char i = 0; i < map_width; i++) {
      for (unsigned char t = 0; t < map_height; t++) {
        const bool edge =
            i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1;

        // Passes 2 to 4 read the neighbours, which edges do not all have
        if (edge && pass >= 2 && pass <= 4) {
          continue;
        }

        // Pass 0 (Initial)
        if (pass == 0) {
          map_temp[i][t] = BarrierType::NONE;
        }
        // Pass 1 (Edges)
        else if (pass == 1) {
          if (edge) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 2 (Well Placed blocks)
        else if (pass == 2) {
          if (map_temp[i - 1][t] == BarrierType::NONE &&
              map_temp[i + 1][t] == BarrierType::NONE &&
              map_temp[i - 1][t + 1] == BarrierType::NONE &&
              map_temp[i + 1][t + 1] == BarrierType::NONE &&
              map_temp[i - 1][t - 1] == BarrierType::NONE &&
              map_temp[i + 1][t - 1] == BarrierType::NONE &&
              map_temp[i][t - 1] == BarrierType::NONE &&
              map_temp[i][t + 1] == BarrierType::NONE &&
              asw::random::between(0, 2) == 1) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 3 (Filling)
        else if (pass == 3) {
          if ((map_temp[i - 1][t] == BarrierType::STONE &&
               map_temp[i + 1][t] == BarrierType::STONE) ||
              (map_temp[i][t - 1] == BarrierType::STONE &&
               map_temp[i][t + 1] == BarrierType::STONE)) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 4 (Filling inaccessible areas)
        else if (pass == 4) {
          if (map_temp[i - 1][t] == BarrierType::STONE &&
              map_temp[i + 1][t] == BarrierType::STONE &&
              map_temp[i][t - 1] == BarrierType::STONE &&
              map_temp[i][t + 1] == BarrierType::STONE) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 5 (Boxes!)
        else if (pass == 5) {
          if (map_temp[i][t] == BarrierType::NONE &&
              asw::random::between(1, 20) == 1) {
            map_temp[i][t] = BarrierType::BOX;
          }
        }
        // Pass 6 (Find start locations)
        else if (pass == 6) {
          if (map_temp[i][t] == BarrierType::NONE) {
            startLocations.emplace_back(i * 40, t * 40);
          }
        }
        // Pass 7 (create barriers)
        else if (pass == 7 && map_temp[i][t] != BarrierType::NONE) {
          auto position = asw::Vec2<float>(i * 40, t * 40);
          auto barrier = create_object<Barrier>(this, position, map_temp[i][t]);

          if (edge) {
            barrier->makeIndestructible(true);
          }
        }
      }
    }

    // No room for the tanks, start again
    if (pass == 6 && startLocations.empty()) {
      pass = -1;
    }
  }

  // Player
  {
    const auto startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = create_object<PlayerTank>(this, camera, startLocation, 100,
                                          500.0F, 0.7F, 125.0F);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
    camera.snap_to(tank->transform.get_center());
  }

  // Enemies, one more each round
  const int enemies = num_enemies + currentRound - 1;
  for (int i = 0; i < enemies; i++) {
    const auto startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = create_object<AiTank>(
        this, camera, startLocation, asw::random::between(50, 150),
        asw::random::between(1, 8) * 125.0F, asw::random::between(0.5F, 1.5F),
        125.0F, 1);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
  }

  // Friends
  for (unsigned char i = 0; i < num_friends; i++) {
    const auto startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = create_object<AiTank>(this, camera, startLocation, 100, 500.0F,
                                      1.0F, 125.0F, 0);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
  }
}