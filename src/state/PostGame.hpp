#pragma once

#include <asw/asw.h>

#include "../game/GameStats.hpp"
#include "../ui/Button.hpp"
#include "../ui/Label.hpp"
#include "./State.hpp"

class PostGame : public asw::scene::Scene<States> {
public:
    using asw::scene::Scene<States>::Scene;

    void init() override;
    void update(float dt) override;
    void draw() override;

private:
    std::shared_ptr<Button> play_again;
    std::shared_ptr<Button> menu_button;

    asw::Font font;
    asw::Font header_font;
};
