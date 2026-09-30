#pragma once

#include <asw/asw.h>
#include <memory>
#include <vector>

#include "./Barrier.hpp"
#include "./Bullet.hpp"
#include "./Particle.hpp"
#include "./PowerUp.hpp"

class Tank : public asw::game::GameObject {
 public:
  explicit Tank(asw::scene::Scene<States>* scene,
                asw::Camera& camera,
                const asw::Vec2<float>& position,
                int health,
                float fireSpeed,
                float fireDelay,
                float speed,
                int team);

  virtual ~Tank() = default;

  void update(float dt) override;
  void draw() override;
  virtual void putDecal();

  int getTeam() const { return team; }

  virtual void set_map_dimensions(int mWidth, int mHeight) {
    map_width = mWidth;
    map_height = mHeight;
  }

  virtual void pickupPowerUp(PowerUpType type);

  static unsigned char num_bullet_bounces;

 protected:
  int health;
  int initialHealth;

  // Bullet speed in pixels per second
  float fire_speed;

  // Seconds between shots
  float fire_delay_rate;

  // Pixels per second
  float max_speed;
  float speed = 0;

  asw::Texture image_base;
  asw::Texture image_hurt;
  asw::Texture image_top;
  asw::Texture image_treads;

  asw::scene::Scene<States>* scene;
  asw::Camera& camera;

  float rotation_body = 0;
  float rotation_turret = 0;

  float bullet_delay = 0;

  int map_width;
  int map_height;

  bool canMoveX = true;
  bool canMoveY = true;

  int team;

  // Update
  void drive(float rotation, float dt);
  void shoot(float rotation, const asw::Vec2<float>& target);
  void accelerate(bool moving, float dt);

 private:
  virtual void collideBullets(float dt);
  virtual void collideBarriers(float dt);
  virtual void collidePowerUps();

  // Update
  void explode();

  // Draw
  void drawTankBase();
  void drawTankTurret();
  void drawHealthBar(float x, float y, int width, int height, int border) const;
};
