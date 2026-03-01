#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../state/State.hpp"
#include "../system/ImageRegistry.hpp"
#include "../system/SampleRegistry.hpp"
#include "GameStats.hpp"

unsigned char Tank::num_bullet_bounces = 0;

Tank::Tank(asw::scene::Scene<States>* scene, const asw::Vec2<float>& position, int health,
    int fireSpeed, float fireDelay, float speed, int team)
    : health(health)
    , initialHealth(health)
    , fire_speed(fireSpeed)
    , fire_delay_rate(fireDelay)
    , max_speed(speed)
    , image_base(nullptr)
    , image_hurt(nullptr)
    , image_top(nullptr)
    , image_treads(nullptr)
    , scene(scene)
    , team(team)
{
    transform.position = position;

    // Map size
    auto screenSize = asw::display::get_size();
    map_width = screenSize.x;
    map_height = screenSize.y;
    z_index = 5;
}

// Explode
void Tank::explode()
{
    for (int i = 0; i < 200; i++) {
        scene->create_object<Particle>(scene, transform.get_center(),
            asw::Color(255, asw::random::between(0, 255), 0), -2.0F, 2.0F, -2.0F, 2.0F, 4,
            ParticleType::SQUARE, 200, ParticleBehaviour::FIRE);
    }
}

void Tank::accelerate(bool moving, float dt)
{
    if (moving) {
        if (speed < 0.1F) {
            speed = 0.2F;
        } else if (speed < max_speed) {
            speed *= (max_speed * 1.03F) * dt * 125.0F;
        } else {
            speed = max_speed;
        }
    } else {
        if (speed > 0.1F) {
            speed /= 1.05F * dt * 125.0F;
        } else {
            speed = 0;
        }
    }
}

// Check collision
void Tank::collideBullets(float dt)
{
    for (auto const& obj : scene->get_object_view<Bullet>()) {
        if (obj->getTeam() == team) {
            continue;
        }

        const auto objTrans = obj->transform
            + asw::Quad<float>(
                obj->getXVelocity() * dt * 125.0F, obj->getYVelocity() * dt * 125.0F, 0, 0);

        if (transform.collides(objTrans)) {
            health -= 10;
            obj->destroy();
            GameStats::instance.hits++;
            GameStats::instance.damage_taken += 10;
            GameStats::instance.damage_done += 10;
        }
    }
}

void Tank::collideBarriers(float dt)
{
    const float delta_speed = speed * dt * 125.0F;
    const float guess_vector_x = -delta_speed * cosf(rotation_body);
    const float guess_vector_y = -delta_speed * sinf(rotation_body);
    const auto offsetXPos = transform + asw::Quad<float>(2 + guess_vector_x, 2, -2, -2);
    const auto offsetYPos = transform + asw::Quad<float>(2, 2 + guess_vector_y, -2, -2);

    canMoveX = true;
    canMoveY = true;

    for (auto const& obj : scene->get_object_view<Barrier>()) {
        if (offsetXPos.collides(obj->transform)) {
            canMoveX = false;
        }
        if (offsetYPos.collides(obj->transform)) {
            canMoveY = false;
        }
    }
}

void Tank::collidePowerUps()
{
    for (auto const& obj : scene->get_object_view<PowerUp>()) {
        if (transform.collides(obj->transform)) {
            pickupPowerUp(obj->getType());
            obj->pickup();
            GameStats::instance.powerups_picked_up++;
        }
    }
}

// Move around
void Tank::drive(float rotation, float dt)
{
    const float deltaSpeed = speed * dt * 125.0F;

    if (canMoveX) {
        transform.position.x += -deltaSpeed * cosf(rotation);
    }
    if (canMoveY) {
        transform.position.y += -deltaSpeed * sinf(rotation);
    }
}

// Shoot
void Tank::shoot(float rotation, const asw::Vec2<float>& target)
{
    if (bullet_delay <= 0) {
        asw::sound::play(SampleRegistry::getSample("fire"), 255, 127, 0);

        scene->create_object<Bullet>(
            scene, target.x, target.y, rotation, fire_speed, 1 + num_bullet_bounces, team);
        GameStats::instance.bullets_fired++;

        bullet_delay = fire_delay_rate;
    }
}

// Update
void Tank::update(float dt)
{
    // Collides
    collidePowerUps();
    collideBarriers(dt);
    collideBullets(dt);

    // Just died
    if (alive && (health <= 0)) {
        explode();
        asw::sound::play(SampleRegistry::getSample("tank-explode"), 255, 127, 0);
        alive = false;
    }

    bullet_delay -= dt * 1000.0F;
}

// Draw Tank
void Tank::drawTankBase()
{
    // Hurt image for player
    if (!alive) {
        asw::draw::rotate_sprite(image_hurt, transform.position, rotation_body);
    } else {
        asw::draw::rotate_sprite(image_base, transform.position, rotation_body);
    }
}

// Draw turret
void Tank::drawTankTurret()
{
    if (!alive) {
        return;
    }

    asw::draw::rotate_sprite(image_top, transform.position, rotation_turret);
}

// Draw health
void Tank::drawHealthBar(float x, float y, int width, int height, int border) const
{
    if (health >= initialHealth || !alive) {
        return;
    }

    const float healthPercent = static_cast<float>(health) / static_cast<float>(initialHealth);

    asw::draw::rect_fill(asw::Quad<float>(x, y, width, height), asw::Color(0, 0, 0));
    asw::draw::rect_fill(asw::Quad<float>(x + border, y + border, width - border, height - border),
        asw::Color(255, 0, 0));
    asw::draw::rect_fill(
        asw::Quad<float>(x + border, y + border, (healthPercent * width) - border, height - border),
        asw::Color(0, 255, 0));
}

// Draw
void Tank::draw()
{
    // Tank
    drawTankBase();

    // Turret
    drawTankTurret();

    // Health bar
    drawHealthBar(transform.position.x - 5, transform.position.y - 10, 50, 6, 1);
}

// Put decals
void Tank::putDecal()
{
    if (alive && speed > 0) {
        asw::draw::rotate_sprite(image_treads,
            asw::Vec2<float>(transform.get_center().x, transform.position.y), rotation_turret);
    }
}

// Power ups
void Tank::pickupPowerUp(PowerUpType type)
{
    switch (type) {
    case PowerUpType::HEALTH: {
        const auto healthBefore = health;
        health = std::min(health + 25, 100);

        if (const int effective = health - healthBefore; effective > 0) {
            GameStats::instance.healing_done += effective;
        }
        break;
    }
    case PowerUpType::SPEED:
        max_speed += 0.25F;
        break;
    case PowerUpType::FIRE_SPEED:
        fire_speed += 1;
        break;
    case PowerUpType::FIRE_DELAY:
        fire_delay_rate -= std::max(10.0F, fire_delay_rate * 0.5F);
        break;
    default:
        break;
    }
}
