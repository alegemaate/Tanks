
#pragma once

#include <string>

class Sprite final {
 public:
  Sprite(const std::string key) noexcept;

  Sprite(Sprite&&) noexcept;

  ~Sprite() noexcept = default;

  Sprite& operator=(Sprite&&) noexcept;

 public:
  std::string key;
};
