#include <filesystem>

#include "Player.h"

int main() {
  std::string cwd = std::filesystem::current_path().generic_string();

  sf::SoundBuffer attack_fx;
  if (!attack_fx.loadFromFile(cwd + "/assets/attack.wav"))
    return EXIT_FAILURE;

  sf::Texture player_tex;
  if (!player_tex.loadFromFile(cwd + "/assets/player.png"))
    return EXIT_FAILURE;

  // player animation data loading is extremely clunky; find an automated way to
  // do this using files
  DirectionMap player_idle = {
      {UP, StateInfo(sf::Vector2i(1, 3), 1)},
      {DOWN, StateInfo(sf::Vector2i(1, 0), 1)},
      {LEFT, StateInfo(sf::Vector2i(1, 2), 1)},
      {RIGHT, StateInfo(sf::Vector2i(1, 1), 1)},
  };

  DirectionMap player_walk = {
      {UP, StateInfo(sf::Vector2i(0, 3), 3)},
      {DOWN, StateInfo(sf::Vector2i(0, 0), 3)},
      {LEFT, StateInfo(sf::Vector2i(0, 2), 3)},
      {RIGHT, StateInfo(sf::Vector2i(0, 1), 3)},
  };

  DirectionMap player_attack = {
      {UP, StateInfo(sf::Vector2i(3, 3), 3)},
      {DOWN, StateInfo(sf::Vector2i(3, 0), 3)},
      {LEFT, StateInfo(sf::Vector2i(3, 2), 3)},
      {RIGHT, StateInfo(sf::Vector2i(3, 1), 3)},
  };

  StateMap player_states = {
      {IDLE, player_idle}, {WALK, player_walk}, {ATTACK, player_attack}};

  SoundMap player_sounds = {{ATTACK, attack_fx}};

  sf::VideoMode vm(800, 600);
  sf::RenderWindow window(vm, "RPG");

  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  sf::Clock dt_clock;
  float dt;

  Player player(player_tex, sf::Vector2i(32, 32), player_states, player_sounds);

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
