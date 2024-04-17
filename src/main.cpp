#include <filesystem>

#include "Animator.h"

int main() {
  std::string cwd(std::filesystem::current_path().generic_string());

  sf::SoundBuffer attack_fx;
  if (!attack_fx.loadFromFile(cwd + "/assets/attack.wav"))
    return EXIT_FAILURE;

  sf::Texture player_tex;
  if (!player_tex.loadFromFile(cwd + "/assets/Jeremy/Jeremy_Blonde.png"))
    return EXIT_FAILURE;


  std::array<Animation, MAX_ANIMS> player_animations = {
    Animation(sf::Vector2i(0, 3), sf::Vector2i(32, 32), 3), // walk up
    Animation(sf::Vector2i(0, 0), sf::Vector2i(32, 32), 3), // walk down
    Animation(sf::Vector2i(0, 2), sf::Vector2i(32, 32), 3), // walk left
    Animation(sf::Vector2i(0, 1), sf::Vector2i(32, 32), 3)  // walk right
  };

  sf::VideoMode vm(800, 600);
  sf::RenderWindow window(vm, "RPG");

  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  sf::Clock dt_clock;
  float dt;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // event-dependent code here
    }

    dt = dt_clock.restart().asSeconds();

    window.clear(sf::Color::Black);

    // draw code here

    window.display();
  }

  return 0;
}
