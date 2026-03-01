#include "Sprite.hpp"
#include "../system/ImageRegistry.hpp"

Sprite::Sprite(const std::string key) noexcept
    : key(key)
{
}

Sprite::Sprite(Sprite&& s) noexcept
{
    this->key = s.key;
}

Sprite& Sprite::operator=(Sprite&& s) noexcept
{
    if (this != &s) {
        this->key = s.key;
    }

    return *this;
}
