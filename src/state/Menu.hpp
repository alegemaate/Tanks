/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#pragma once

#include "../ui/Button.hpp"
#include "Game.hpp"
#include "State.hpp"

class Menu : public asw::scene::Scene<States> {
public:
    using asw::scene::Scene<States>::Scene;

    void init() override;
    void update(float dt) override;
    void draw() override;

private:
    asw::Font font;
    asw::ui::Root ui_root;
};
