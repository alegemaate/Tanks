
#include <entt/entt.hpp>

#include "../components/Sprite.hpp"
#include "../components/Transform.hpp"

#include "../system/ImageRegistry.hpp"

#include "RenderSystem.hpp"

void RenderSystem::render(BITMAP* buffer, entt::registry& registry) {
  auto view = registry.view<Sprite, Transform>();

  view.each([&](const auto& spr, auto& pos) {
    auto* bitmap = ImageRegistry::getImage(spr.key);
    draw_sprite(buffer, bitmap, pos.x, pos.y);
  });
}
