
#include <entt/entt.hpp>

#include "../components/Sprite.hpp"
#include "../components/Transform.hpp"

#include "../system/ImageRegistry.hpp"

#include "RenderSystem.hpp"

void RenderSystem::render(entt::registry& registry) {
  auto view = registry.view<Sprite, Transform>();

  view.each([&](const auto& spr, auto& pos) {
    auto bitmap = ImageRegistry::getImage(spr.key);
    asw::draw::sprite(bitmap, asw::Vec2<float>(pos.x, pos.y));
  });
}
