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
                const asw::Vec2<float>& position,
                int health,
                int fireSpeed,
                int fireDelay,
                float speed,
                int team);

  virtual ~Tank() = default;

  void update(float deltaTime) override;
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
  int fire_speed;
  int fire_delay_rate;

  float max_speed;
  float speed = 0;

  asw::Texture image_base;
  asw::Texture image_hurt;
  asw::Texture image_top;
  asw::Texture image_treads;

  asw::scene::Scene<States>* scene;

  float rotation_body = 0;
  float rotation_turret = 0;

  int bullet_delay = 0;

  int map_width;
  int map_height;

  bool canMoveX = true;
  bool canMoveY = true;

  int team;

  // Update
  void drive(float rotation, float deltaTime);
  void shoot(float rotation, const asw::Vec2<float>& target);
  void accelerate(bool moving, float deltaTime);

 private:
  virtual void collideBullets(float deltaTime);
  virtual void collideBarriers(float deltaTime);
  virtual void collidePowerUps();

  // Update
  void explode();

  // Draw
  void drawTankBase();
  void drawTankTurret();
  void drawHealthBar(float x, float y, int width, int height, int border) const;
};
