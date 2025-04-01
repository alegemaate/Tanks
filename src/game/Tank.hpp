#ifndef SRC_GAME_TANK_H_
#define SRC_GAME_TANK_H_

#include <asw/asw.h>
#include <memory>
#include <vector>

#include "./Barrier.hpp"
#include "./Bullet.hpp"
#include "./Particle.hpp"
#include "./PowerUp.hpp"
#include "./World.hpp"

class Tank {
 public:
  explicit Tank(World* worldPointer,
                float x,
                float y,
                int health,
                int fireSpeed,
                int fireDelay,
                float speed);

  virtual ~Tank() = default;

  virtual bool isDead() const;

  virtual std::vector<Bullet>& getBullets();

  virtual void update(float deltaTime);
  virtual void draw();
  virtual void putDecal();

  virtual float getX() const { return position.x; }
  virtual float getY() const { return position.y; }

  virtual float getCenterX() const { return position.x + width / 2.0F; }
  virtual float getCenterY() const { return position.y + height / 2.0F; }

  virtual void set_map_dimensions(int mWidth, int mHeight) {
    map_width = mWidth;
    map_height = mHeight;
  }

  virtual void checkCollision(std::vector<Bullet>& enemyBullets,
                              float deltaTime);
  virtual void checkCollision(const std::vector<Barrier>& barriers,
                              float deltaTime);
  virtual void checkCollision(std::vector<PowerUp>& power_ups, float deltaTime);

  virtual void process_enemies(std::vector<std::shared_ptr<Tank>>* otherTanks);

  virtual void pickupPowerUp(PowerUpType type);

  static unsigned char num_bullet_bounces;

 protected:
  asw::Vec2<float> position;

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

  World* worldPointer;

  bool dead = false;

  float rotation_body = 0;
  float rotation_turret = 0;

  int bullet_delay = 0;

  float width = 0;
  float height = 0;

  int map_width;
  int map_height;

  bool canMoveX = true;
  bool canMoveY = true;

  std::vector<std::shared_ptr<Tank>>* otherTanks;

  // Update
  void drive(float rotation, float deltaTime);
  void shoot(float rotation, float targetX, float targetY);
  void accelerate(bool moving, float deltaTime);

 private:
  std::vector<Bullet> bullets;

  static asw::Sample sample_shot;

  // Update
  void update_bullets(float deltaTime);
  void explode();

  // Draw
  void drawBullets() const;
  void drawTankBase();
  void drawTankTurret();
  void drawHealthBar(float x, float y, int width, int height, int border) const;
};

#endif  // SRC_GAME_TANK_H_
