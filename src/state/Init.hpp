/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#pragma once

#include <asw/asw.h>

#include "./Game.hpp"
#include "./Menu.hpp"
#include "./PostGame.hpp"
#include "./State.hpp"

class Init : public asw::scene::Scene<States> {
public:
    using asw::scene::Scene<States>::Scene;

    void init() override;

    void update(float _dt) override
    {
        manager.register_scene<Menu>(States::Menu, manager);
        manager.register_scene<Game>(States::Game, manager);
        manager.register_scene<PostGame>(States::PostGame, manager);

        // Goto menu
        manager.set_next_scene(States::Menu);
    }
};
