#include <asw/asw.h>

#include "./state/Game.hpp"
#include "./state/Init.hpp"
#include "./state/Menu.hpp"
#include "./state/State.hpp"

// Main function*/
int main() {
  asw::core::init(800, 600);

  auto app = asw::scene::SceneManager<States>();
  app.register_scene<Init>(States::Init, app);
  app.set_next_scene(States::Init);
  app.start();

  return 0;
}
