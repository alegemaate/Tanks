#include "Game.hpp"

#include <algorithm>

#include "../game/GameStats.hpp"
#include "../system/ImageRegistry.hpp"
#include "../system/SampleRegistry.hpp"

unsigned char Game::map_width = 20;
unsigned char Game::map_height = 20;

unsigned char Game::num_enemies = 5;
unsigned char Game::num_friends = 5;

const unsigned char max_map_width = 255;
const unsigned char max_map_height = 255;

// Init state (and Game)
void Game::init()
{
    // Reset vars
    currentRound = 0;
    timer = 0.0f;
    GameStats::instance.reset();

    // Create buffers
    decal_buffer = asw::assets::create_texture(map_width * 40, map_height * 40);
    map_buffer = asw::assets::create_texture(map_width * 40, map_height * 40);
    light_buffer = asw::assets::create_texture(map_width * 40, map_height * 40);
    fade_buffer = asw::assets::create_texture(map_width * 40, map_height * 40);

    // Setup decal buffer
    asw::display::set_render_target(decal_buffer);
    asw::draw::clear_color(asw::Color(0, 0, 0, 0));
    asw::display::reset_render_target();

    // Setup light buffer
    asw::draw::set_blend_mode(light_buffer, asw::BlendMode::Add);
    asw::draw::set_alpha(light_buffer, 0.5F);

    // Setup fade buffer
    asw::display::set_render_target(fade_buffer);
    asw::draw::clear_color(asw::Color(0, 0, 0, 10));
    asw::display::reset_render_target();
    asw::draw::set_blend_mode(fade_buffer, asw::BlendMode::Blend);

    // Load images
    background = ImageRegistry::getImage("game-background");
    cursor = ImageRegistry::getImage("cursor");

    // Font
    font = asw::assets::load_font("assets/fonts/ariblk.ttf", 12);

    // Create map
    generateMap();
}

void Game::update(float dt)
{
    // Update world
    Scene::update(dt);

    timer += dt * 1000.0F;

    // Check win/lose conditions
    bool hasLivingFriendlies = false;
    bool hasLivingEnemies = false;

    for (const auto& tank : get_object_view<Tank>()) {
        if (!tank->alive) {
            continue;
        }
        if (tank->getTeam() == 0) {
            hasLivingFriendlies = true;
        } else if (tank->getTeam() == 1) {
            hasLivingEnemies = true;
        }
    }

    if (!hasLivingEnemies) {
        GameStats::instance.victory = true;
        GameStats::instance.round_duration = timer;
        asw::sound::play(SampleRegistry::getSample("tank-explode"), 255, 127, 0);
        manager.set_next_scene(States::PostGame);
        return;
    }

    if (!hasLivingFriendlies) {
        GameStats::instance.victory = false;
        GameStats::instance.round_duration = timer;
        asw::sound::play(SampleRegistry::getSample("explode"), 255, 127, 0);
        manager.set_next_scene(States::PostGame);
        return;
    }

    // Return to menu
    if (asw::input::get_key_down(asw::input::Key::M)) {
        manager.set_next_scene(States::Menu);
    }

    // Scroll map
    for (const auto& tank : get_object_view<PlayerTank>()) {
        auto screen_size = asw::display::get_size();
        map_position = tank->transform.get_center()
            - asw::Vec2<float>(screen_size.x / 2.0F, screen_size.y / 2.0F);
    }
}

void Game::draw()
{
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
    asw::draw::clear_color(asw::Color(0, 88, 0, 255));

    // Decal to buffer
    asw::draw::sprite(decal_buffer, asw::Vec2<float>(0, 0));

    // Draw world
    Scene::draw();

    // Light to buffer
    asw::draw::set_blend_mode(light_buffer, asw::BlendMode::Multiply);
    asw::draw::sprite(light_buffer, asw::Vec2<float>(0, 0));

    // Map to buffer
    asw::display::reset_render_target();
    asw::draw::sprite(map_buffer, map_position * -1.0F);

    // Text
    asw::draw::text(font, std::format("Round: {}", currentRound), asw::Vec2<float>(20, 20),
        asw::Color(255, 255, 255));
    // asw::draw::text(font, "Team BLUE: " + std::to_string(player_tanks.size()),
    //                 asw::Vec2<float>(20, 35),
    //                 asw::Color(255, 255, 255));
    // asw::draw::text(font, "Team RED: " + std::to_string(enemy_tanks.size()),
    //                 asw::Vec2<float>(20, 50),
    //                 asw::Color(255, 255, 255));

    // Cursor
    asw::draw::sprite(cursor, asw::input::mouse.position + asw::Vec2<float>(-10, -10));
}

void Game::generateMap()
{
    // Make a map
    std::array<std::array<BarrierType, max_map_height>, max_map_width> map_temp {};
    std::vector<asw::Vec2<float>> startLocations {};

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
                    if (map_temp[i - 1][t] == BarrierType::NONE
                        && map_temp[i + 1][t] == BarrierType::NONE
                        && map_temp[i - 1][t + 1] == BarrierType::NONE
                        && map_temp[i + 1][t + 1] == BarrierType::NONE
                        && map_temp[i - 1][t - 1] == BarrierType::NONE
                        && map_temp[i + 1][t - 1] == BarrierType::NONE
                        && map_temp[i][t - 1] == BarrierType::NONE
                        && map_temp[i][t + 1] == BarrierType::NONE
                        && asw::random::between(0, 2) == 1) {
                        map_temp[i][t] = BarrierType::STONE;
                    }
                }
                // Pass 3 (Filling)
                else if (pass == 3) {
                    if ((map_temp[i - 1][t] == BarrierType::STONE
                            && map_temp[i + 1][t] == BarrierType::STONE)
                        || (map_temp[i][t - 1] == BarrierType::STONE
                            && map_temp[i][t + 1] == BarrierType::STONE)) {
                        map_temp[i][t] = BarrierType::STONE;
                    }
                }
                // Pass 4 (Filling inaccessible areas)
                else if (pass == 4) {
                    if (map_temp[i - 1][t] == BarrierType::STONE
                        && map_temp[i + 1][t] == BarrierType::STONE
                        && map_temp[i][t - 1] == BarrierType::STONE
                        && map_temp[i][t + 1] == BarrierType::STONE) {
                        map_temp[i][t] = BarrierType::STONE;
                    }
                }
                // Pass 5 (Boxes!)
                else if (pass == 5) {
                    if (map_temp[i][t] == BarrierType::NONE && asw::random::between(1, 20) == 1) {
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

                    if (i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1) {
                        barrier->makeIndestructible(true);
                    }
                }
            }
        }
    }

    // Player
    {
        const auto startLocation = startLocations.at(
            asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
        auto tank = create_object<PlayerTank>(this, startLocation, 100, 4, 700, 1);
        tank->set_map_dimensions(map_width * 40, map_height * 40);
    }

    // Enemies
    for (unsigned char i = 0; i < num_enemies; i++) {
        const auto startLocation = startLocations.at(
            asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
        auto tank = create_object<AiTank>(this, startLocation, asw::random::between(50, 150),
            asw::random::between(1, 8), asw::random::between(500.0F, 1500.0F), 1, true);
        tank->set_map_dimensions(map_width * 40, map_height * 40);
    }

    // Friends
    for (unsigned char i = 0; i < num_friends; i++) {
        const auto startLocation = startLocations.at(
            asw::random::between(0, static_cast<int>(startLocations.size()) - 1));
        auto tank = create_object<AiTank>(this, startLocation, 100, 4, 1000, 1, false);
        tank->set_map_dimensions(map_width * 40, map_height * 40);
    }
}