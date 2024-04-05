#include "Player.h"

#include <iostream>

void Player::onKeypressInput(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    vel.y = -1;

    setState(WALK);

    // left/right animations when going diagonally look better :)
    if (vel.x == 0)
      setDir(UP);
  }

  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    vel.y = 1;

    setState(WALK);

    if (vel.x == 0)
      setDir(DOWN);
  }

  else
    vel.y = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    vel.x = -1;

    setState(WALK);
    setDir(LEFT);
  }

  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    vel.x = 1;

    setState(WALK);
    setDir(RIGHT);
  }

  else
    vel.x = 0;

  if (vel.x == 0 && vel.y == 0)
    setState(IDLE);

  pos += static_cast<sf::Vector2f>(vel) * base_speed * speed_multiplier * dt;

  sprite.setPosition(pos);
}

void Player::stepFrame(float dt) {
  if (curr_state == NONE)
    return;

  frame_timer += dt;

  // onKeypressInput frames, if reach last frame, go back to first one
  if (frame_timer >= 1.0f / fps) {
    if (++curr_frame == curr_frame_count) {
      if (in_action)
        endAction();
      curr_frame = 0;
    }

    frame_timer = 0;
  }
}

void Player::startAction(State action) {

  setState(action, true);
  in_action = true;
}

void Player::endAction() {
  in_action = false;
  setState(IDLE);
}

void Player::playSound(State source) {
  if (sfx.find(curr_state) != sfx.end()) {
    sound.setBuffer(sfx.at(curr_state));
  }

  sound.play();
}

void Player::setState(State new_state, bool reset) {
  if ((curr_state == new_state || in_action) && !reset)
    return;

  curr_state = new_state;

  speed_multiplier =
      speed_multipliers.find(curr_state) == speed_multipliers.end()
          ? 1
          : speed_multipliers.at(curr_state);

  StateInfo target = getStateInfo(curr_state, curr_dir);

  curr_frame_count = target.frame_count;
  curr_frame = 0;
}

void Player::setDir(Direction new_dir) {
  if (curr_dir == new_dir) {
    return;
  }

  curr_dir = new_dir;

  // actions are one-time animations; we should never reset them
  if (!in_action)
    curr_frame = 0;
}

void Player::animate() {
  if (curr_state == NONE)
    return;

  StateInfo target = getStateInfo(curr_state, curr_dir);

  // draw rect tracks current frame; must be done continuously
  draw_rect.left = (target.coords.x + curr_frame) * frame_dimensions.x;
  draw_rect.top = target.coords.y * frame_dimensions.y;

  sprite.setTextureRect(draw_rect);
}

DirectionMap &Player::getDirectionMap(State state) { return states.at(state); }

StateInfo &Player::getStateInfo(State state, Direction dir) {
  return states.at(state).at(dir);
}

Player::Player(sf::Texture &spritesheet, sf::Vector2i frame_dimensions,
               StateMap &states, SoundMap &sfx)
    : states(states), sfx(sfx) {
  this->frame_dimensions = frame_dimensions;
  this->states = states;
  this->sfx = sfx;

  draw_rect.width = frame_dimensions.x;
  draw_rect.height = frame_dimensions.y;

  sprite.setScale(sf::Vector2f(scale, scale));
  sprite.setTexture(spritesheet);
}

void Player::onEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.scancode) {
    case sf::Keyboard::Scan::C:
      startAction(ATTACK);
      playSound(ATTACK);
      break;
    default:
      break;
    }
  }
}

void Player::draw(sf::RenderWindow &window) {
  // draw sprite to window
  window.draw(sprite);
}

void Player::update(float dt) {
  onKeypressInput(dt);
  stepFrame(dt);
  animate();
  // debug
  std::cout << "State, Direction, Frame/Total: " << curr_state << ", "
            << curr_dir << ", " << curr_frame << "/" << curr_frame_count
            << std::endl;
}
