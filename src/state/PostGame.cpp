#include "PostGame.hpp"

#include <string>

static std::string formatDuration(float ms)
{
    const int totalSeconds = static_cast<int>(ms / 1000.0F);
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;
    return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

void PostGame::init()
{
    const auto& stats = GameStats::instance;

    header_font = asw::assets::load_font("assets/fonts/ariblk.ttf", 36);
    font = asw::assets::load_font("assets/fonts/ariblk.ttf", 18);

    auto white = asw::Color(255, 255, 255);
    auto gold = asw::Color(255, 215, 0);
    auto red = asw::Color(255, 80, 80);

    // Header
    std::string headerText = stats.victory ? "Victory!" : "Defeat";
    auto headerColor = stats.victory ? gold : red;
    create_object<Label>(300.0F, 40.0F, headerText, header_font, headerColor);

    // Stats
    float y = 130.0F;
    const float labelX = 200.0F;
    const float valueX = 550.0F;
    const float spacing = 35.0F;

    create_object<Label>(labelX, y, "Round Duration:", font, white);
    create_object<Label>(valueX, y, formatDuration(stats.round_duration), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Bullets Fired:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.bullets_fired), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Hits:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.hits), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Damage Done:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.damage_done), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Damage Taken:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.damage_taken), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Healing Done:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.healing_done), font, white);
    y += spacing;

    create_object<Label>(labelX, y, "Powerups Picked Up:", font, white);
    create_object<Label>(valueX, y, std::to_string(stats.powerups_picked_up), font, white);

    // Buttons
    play_again = create_object<Button>(250.0F, 480.0F, "Play Again", font);
    menu_button = create_object<Button>(430.0F, 480.0F, "Menu", font);
}

void PostGame::update(float dt)
{
    Scene::update(dt);

    if (play_again->clicked()) {
        manager.set_next_scene(States::Game);
    }

    if (menu_button->clicked()) {
        manager.set_next_scene(States::Menu);
    }
}

void PostGame::draw()
{
    asw::draw::rect_fill(asw::Quad<float>(0, 0, 800, 600), asw::Color(20, 20, 30));
    Scene::draw();
}
