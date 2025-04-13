#pragma once

class Transform final {
 public:
  Transform(const double x, const double y) noexcept;

  Transform(Transform&&) noexcept;

  ~Transform() noexcept = default;

  Transform& operator=(Transform&&) noexcept;

 public:
  double x;

  double y;
};
