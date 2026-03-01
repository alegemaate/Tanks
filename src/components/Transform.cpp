
#include "Transform.hpp"

Transform::Transform(const double x, const double y) noexcept
    : x { x }
    , y { y }
{
}

Transform::Transform(Transform&& p) noexcept
{
    this->x = p.x;
    this->y = p.y;
}

Transform& Transform::operator=(Transform&& p) noexcept
{
    if (this != &p) {
        this->x = p.x;
        this->y = p.y;
    }

    return *this;
}
