#include <filesystem>

#include "Player.h"

int main() {
  std::string cwd(std::filesystem::current_path().generic_string());

  sf::SoundBuffer attack_fx;
  if (!attack_fx.loadFromFile(cwd + "/assets/attack.wav"))
    return EXIT_FAILURE;

  sf::Texture player_tex;
  if (!player_tex.loadFromFile(cwd + "/assets/Jeremy/Jeremy_Blonde.png"))
    return EXIT_FAILURE;

  Animation player_walk_up{sf::Vector2i(0, 3), sf::Vector2i(32, 32), 3};
  Animation player_walk_down{sf::Vector2i(0, 0), sf::Vector2i(32, 32), 3};
  Animation player_walk_left{sf::Vector2i(0, 2), sf::Vector2i(32, 32), 3};
  Animation player_walk_right{sf::Vector2i(0, 1), sf::Vector2i(32, 32), 3};

  AnimationList player_animations{{WALK,
                                   {player_walk_up, player_walk_down,
                                    player_walk_left, player_walk_right}}};

  SoundMap player_sfx{{ATTACK, attack_fx}};

  Animator player_animator(player_tex, player_animations, player_sfx);

  Player player(player_animator);

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

      player.onEvent(event);
    }

    dt = dt_clock.restart().asSeconds();

    window.clear(sf::Color::Black);

    player.draw(window);
    player.update(dt);

    window.display();
  }

  return 0;
}
