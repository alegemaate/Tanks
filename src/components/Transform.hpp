#pragma once

class Transform final {
public:
    Transform(double x, double y) noexcept;

    Transform(Transform&& p) noexcept;

    ~Transform() noexcept = default;

    Transform& operator=(Transform&& p) noexcept;

    double x;
    double y;
};
