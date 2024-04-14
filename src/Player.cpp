#include "Player.h"

void Player::onKeypressInput(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    vel.y = -1;

    animator.setState(WALK, UP);
  }

  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    vel.y = 1;

    animator.setState(WALK, DOWN);
  }

  else
    vel.y = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    vel.x = -1;

    animator.setState(WALK, LEFT);
  }

  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    vel.x = 1;

    animator.setState(WALK, RIGHT);
  }

  else
    vel.x = 0;

  /*
  if (vel.x == 0 && vel.y == 0)
    animator.setState(IDLE);
  */

  pos += static_cast<sf::Vector2f>(vel) * base_speed * speed_multiplier * dt;
}

Player::Player(Animator& animator) : animator(animator) {
  this->animator = animator;
}

void Player::onEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.scancode) {
    case sf::Keyboard::Scan::C:
      animator.startAction(ATTACK);
      animator.playSound(ATTACK);
      break;
    default:
      break;
    }
  }
}

void Player::draw(sf::RenderWindow& window){
  animator.draw(window, pos);
}

void Player::update(float dt){
  onKeypressInput(dt);
  animator.update(dt);
}
