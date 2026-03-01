#include "./Bullet.hpp"

#include <cmath>

#include "../system/ImageRegistry.hpp"
#include "./Barrier.hpp"

// Init
Bullet::Bullet(asw::scene::Scene<States>* scene, float x, float y, float angle, float speed,
    int health, int team)
    : scene(scene)
    , velocity(-speed * cosf(angle), -speed * sinf(angle))
    , team(team)
    , health(health)
{
    transform.position.x = x;
    transform.position.y = y;
    transform.size.x = 5;
    transform.size.y = 5;
    z_index = 1;
    light_buffer = ImageRegistry::getImage("light");
}

// Reverse specified vector
void Bullet::reverseDirection(const std::string& direction)
{
    if (direction == "x") {
        velocity.x = -velocity.x;
    } else if (direction == "y") {
        velocity.y = -velocity.y;
    } else {
        velocity.y = -velocity.y;
        velocity.x = -velocity.x;
    }
}

// Bounce off wall
void Bullet::bounce(BounceDirection direction)
{
    health--;
    incidenceDirection = direction;
}

// Destroy
void Bullet::destroy()
{
    // Has it already died?
    if (!alive) {
        return;
    }

    // Make sure health is 0
    health = 0;
    alive = false;

    // Make explosion
    for (int i = 0; i < 100; i++) {
        auto color = asw::Color(255, asw::random::between(0, 255), 0);

        switch (incidenceDirection) {
        case BounceDirection::BOTTOM: {
            scene->create_object<Particle>(scene, transform.get_center(), color, -5, 5, 0, 3, 2,
                ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
            break;
        }
        case BounceDirection::TOP: {
            scene->create_object<Particle>(scene, transform.get_center(), color, -5, 5, -3, 0, 2,
                ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
            break;
        }
        case BounceDirection::LEFT: {
            scene->create_object<Particle>(scene, transform.get_center(), color, -3, 0, -5, 5, 2,
                ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
            break;
        }
        default: {
            scene->create_object<Particle>(scene, transform.get_center(), color, 0, 3, -5, 5, 2,
                ParticleType::SQUARE, 10, ParticleBehaviour::EXPLODE);
            break;
        }
        }
    }
}

// Update bullets
void Bullet::update(float dt)
{
    // Destroy if out of bounds or health is 0
    if (health <= 0 || transform.position.x < 0 || transform.position.x > 10000
        || transform.position.y < 0 || transform.position.y > 10000) {
        destroy();
    }

    // Move
    transform.position += velocity * dt * 125.0F;

    // Bounce
    for (auto& obj : scene->get_object_view<Barrier>()) {
        if (transform.collides(obj->transform)) {
            if (transform.collides_bottom(obj->transform)) {
                reverseDirection("y");
                bounce(BounceDirection::BOTTOM);
            } else if (transform.collides_top(obj->transform)) {
                reverseDirection("y");
                bounce(BounceDirection::TOP);
            }

            if (transform.collides_left(obj->transform)) {
                reverseDirection("x");
                bounce(BounceDirection::LEFT);
            } else if (transform.collides_right(obj->transform)) {
                reverseDirection("x");
                bounce(BounceDirection::RIGHT);
            }

            obj->hit();
        }
    }
}

// Draw image
void Bullet::draw()
{
    asw::draw::rect_fill(transform, asw::Color(0, 0, 0));
    const auto inner = transform + asw::Quad<float>(1, 1, -1, -1);
    asw::draw::rect_fill(inner, asw::Color(255, 0, 0));
}
