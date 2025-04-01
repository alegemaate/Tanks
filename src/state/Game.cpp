#include "Game.hpp"

#include <algorithm>

#include "../system/ImageRegistry.hpp"

unsigned char Game::map_width = 20;
unsigned char Game::map_height = 20;

unsigned char Game::num_enemies = 5;
unsigned char Game::num_friends = 5;

const unsigned char max_map_width = 255;
const unsigned char max_map_height = 255;

// Init state (and Game)
void Game::init() {
  // Reset vars
  barriers.clear();
  enemy_tanks.clear();
  player_tanks.clear();
  power_ups.clear();
  map_x = 0;
  map_y = 0;
  currentRound = 0;

  // Create buffers
  decal_buffer = asw::assets::createTexture(map_width * 40, map_height * 40);
  map_buffer = asw::assets::createTexture(map_width * 40, map_height * 40);

  // Setup decal buffer
  asw::display::setRenderTarget(decal_buffer);
  asw::draw::clearColor(asw::util::makeColor(0, 0, 0, 255));
  asw::display::resetRenderTarget();
  asw::draw::setBlendMode(decal_buffer, asw::BlendMode::ADD);

  // Load images
  background = ImageRegistry::getImage("game-background");
  cursor = ImageRegistry::getImage("cursor");

  // Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 12);

  // Make a map
  std::array<std::array<BarrierType, max_map_height>, max_map_width> map_temp{};
  std::vector<asw::Vec2<float>> startLocations{};

  for (unsigned char pass = 0; pass < 8; pass++) {
    for (unsigned char i = 0; i < map_width; i++) {
      for (unsigned char t = 0; t < map_height; t++) {
        // Pass 0 (Initial)
        if (pass == 0) {
          map_temp[i][t] = BarrierType::NONE;
        }
        // Pass 1 (Edges)
        else if (pass == 1) {
          if (i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1) {
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
          auto& barrier =
              barriers.emplace_back(&game_world, position, map_temp[i][t]);

          if (i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1) {
            barrier.makeIndestructible(true);
          }
        }
      }
    }
  }

  // Player
  {
    const asw::Vec2<float> startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = player_tanks.emplace_back(new PlayerTank(
        &game_world, startLocation.x, startLocation.y, 100, 4, 700, 1));

    tank->process_enemies(&enemy_tanks);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
  }

  // Enemies
  for (unsigned char i = 0; i < num_enemies; i++) {
    const asw::Vec2<float> startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = enemy_tanks.emplace_back(
        new AiTank(&game_world, startLocation.x, startLocation.y,
                   asw::random::between(50, 150), asw::random::between(1, 8),
                   asw::random::between(500.0F, 1500.0F), 1, true));

    tank->process_enemies(&player_tanks);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
  }

  // Friends
  for (unsigned char i = 0; i < num_friends; i++) {
    const asw::Vec2<float> startLocation = startLocations.at(
        asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
    auto tank = enemy_tanks.emplace_back(new AiTank(
        &game_world, startLocation.x, startLocation.y, 100, 4, 1000, 1, false));
    tank->process_enemies(&enemy_tanks);
    tank->set_map_dimensions(map_width * 40, map_height * 40);
  }
}

void Game::update(float deltaTime) {
  // Update world
  game_world.update(deltaTime);

  // Move
  for (auto& enemy : enemy_tanks) {
    // Update barriers
    for (auto& barrier : barriers) {
      barrier.update(enemy->getBullets());
    }

    // Update bullets
    for (auto& player : player_tanks) {
      player->checkCollision(enemy->getBullets(), deltaTime);
    }

    // Collision with barrier
    enemy->checkCollision(barriers, deltaTime);

    // Collision with power ups
    enemy->checkCollision(power_ups, deltaTime);

    // Update tanks
    enemy->update(deltaTime);
  }

  for (auto& player : player_tanks) {
    // Update barriers
    for (auto& barrier : barriers) {
      barrier.update(player->getBullets());
    }

    // Update bullets
    for (auto& enemy : enemy_tanks) {
      enemy->checkCollision(player->getBullets(), deltaTime);
    }

    // Collision with barrier
    player->checkCollision(barriers, deltaTime);

    // Collision with power ups
    player->checkCollision(power_ups, deltaTime);

    // Update tanks
    player->update(deltaTime);
  }

  // Remove broken barriers
  for (auto& barrier : barriers) {
    if (barrier.getDead() && asw::random::between(0, 1) == 0) {
      const int randomType = asw::random::between(0, 3);
      PowerUpType type;

      switch (randomType) {
        case 1:
          type = PowerUpType::SPEED;
          break;
        case 2:
          type = PowerUpType::FIRE_SPEED;
          break;
        case 3:
          type = PowerUpType::FIRE_DELAY;
          break;
        default:
          type = PowerUpType::HEALTH;
          break;
      }

      power_ups.emplace_back(barrier.getPosition().x, barrier.getPosition().y,
                             type);
    }
  }

  // Cleanup barriers
  barriers.erase(
      std::remove_if(barriers.begin(), barriers.end(),
                     [](auto const& barrier) { return barrier.getDead(); }),
      barriers.end());

  // Cleanup power_ups
  power_ups.erase(
      std::remove_if(power_ups.begin(), power_ups.end(),
                     [](auto const& power_up) { return power_up.getDead(); }),
      power_ups.end());

  // Game over
  if (asw::input::wasKeyPressed(asw::input::Key::SPACE) &&
      (player_tanks.empty() || enemy_tanks.empty())) {
    sceneManager.setNextScene(States::Menu);
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_M]) {
    sceneManager.setNextScene(States::Menu);
  }

  // Scroll map
  if (!player_tanks.empty()) {
    auto screenSize = asw::display::getSize();
    map_x = player_tanks.at(0)->getCenterX() -
            static_cast<float>(screenSize.x) / 2.0f;
    map_y = player_tanks.at(0)->getCenterY() -
            static_cast<float>(screenSize.y) / 2.0f;
  }
}

void Game::draw() {
  // Draw background
  asw::draw::sprite(background, asw::Vec2<float>(0, 0));

  // Draw decals
  asw::display::setRenderTarget(decal_buffer);

  for (auto enemy : enemy_tanks) {
    enemy->putDecal();
  }

  for (auto player : player_tanks) {
    player->putDecal();
  }

  // Blank map map_buffer
  asw::display::setRenderTarget(map_buffer);
  asw::draw::clearColor(asw::util::makeColor(0, 88, 0, 255));

  // Decal to buffer
  asw::draw::sprite(decal_buffer, asw::Vec2<float>(0, 0));

  // Draw tanks
  for (auto enemy : enemy_tanks) {
    enemy->draw();
  }

  for (auto player : player_tanks) {
    player->draw();
  }

  // Draw world
  game_world.draw();

  // Draw barriers
  for (const auto& barrier : barriers) {
    barrier.draw();
  }

  // Draw power_ups
  for (const auto& power_up : power_ups) {
    power_up.draw();
  }

  // Map to buffer
  asw::display::resetRenderTarget();
  asw::draw::sprite(map_buffer, asw::Vec2<float>(-map_x, -map_y));

  // Text
  asw::draw::text(font, "Round: " + std::to_string(currentRound),
                  asw::Vec2<float>(20, 20),
                  asw::util::makeColor(255, 255, 255));
  asw::draw::text(font, "Team BLUE: " + std::to_string(player_tanks.size()),
                  asw::Vec2<float>(20, 35),
                  asw::util::makeColor(255, 255, 255));
  asw::draw::text(font, "Team RED: " + std::to_string(enemy_tanks.size()),
                  asw::Vec2<float>(20, 50),
                  asw::util::makeColor(255, 255, 255));

  // Cursor
  asw::draw::sprite(cursor, asw::Vec2<float>(asw::input::mouse.x - 10,
                                             asw::input::mouse.y - 10));
}
