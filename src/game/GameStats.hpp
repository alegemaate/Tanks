#pragma once

struct GameStats {
  int bullets_fired = 0;
  int damage_taken = 0;
  int damage_done = 0;
  int hits = 0;
  int healing_done = 0;
  int powerups_picked_up = 0;
  float round_duration = 0.0f;
  bool victory = false;

  void reset() {
    bullets_fired = 0;
    damage_taken = 0;
    damage_done = 0;
    hits = 0;
    healing_done = 0;
    powerups_picked_up = 0;
    round_duration = 0.0f;
    victory = false;
  }

  static GameStats instance;
};
