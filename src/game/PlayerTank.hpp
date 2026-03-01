#pragma once

#include "Tank.hpp"

class PlayerTank : public Tank {
public:
    PlayerTank(asw::scene::Scene<States>* scene, const asw::Vec2<float>& position, int health,
        int fireSpeed, int fireDelay, float speed);

    void update(float dt) override;
};
