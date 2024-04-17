#include "Player.h"

void Player::onKeypressInput(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    vel.y = -1;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    vel.y = 1;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    vel.x = -1;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    vel.x = 1;
  }

  pos += vel * base_speed * speed_multiplier;
}

Player::Player(Animator *animator) {
  this->animator = animator;
}
