#include <iostream>

#include "Animator.h"

Animation::Animation(sf::Vector2i coords, sf::Vector2i frame_dimensions, int frame_count){
  this->coords = coords;
  this->frame_dimensions = frame_dimensions;
  this->frame_count = frame_count;
}

void Animator::stepFrame(float dt) {
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

void Animator::startAction(State action, int anim) {
  setState(action, anim, true);
  in_action = true;
}

void Animator::endAction() {
  in_action = false;
  setState(IDLE, default_anim);
}

void Animator::playSound(State source) {
  if (sfx.find(curr_state) != sfx.end()) {
    sound.setBuffer(sfx.at(curr_state));
  }

  sound.play();
}

void Animator::setState(State new_state, int anim, bool reset) {
  bool state_equals = curr_state == new_state;
  bool anim_equals = curr_anim == anim;

  // don't modify state if repeating same state and animation OR we're in action
  // we may override this using 'reset' to restart actions!
  if (((curr_state == new_state && curr_anim == anim) || in_action) && !reset)
    return;

  curr_state = new_state;
  curr_anim = anim;

  Animation target = getAnim(curr_state, curr_anim);

  draw_rect.width = target.frame_dimensions.x;
  draw_rect.height = target.frame_dimensions.y;
  curr_frame_count = target.frame_count;
  curr_frame = 0;
}

void Animator::setAnim(int idx) {
  if (idx == curr_anim) {
    return;
  }

  curr_anim = idx;

  // actions are one-time animations; we should never reset them
  if (!in_action)
    curr_frame = 0;
}

Animation &Animator::getAnim(State state, int idx) {
  return anims.at(state).at(idx);
}

Animator::Animator(sf::Texture &spritesheet, AnimationList &anims,
                   SoundMap &sfx)
    : anims(anims), sfx(sfx) {
  this->anims = anims;
  this->sfx = sfx;

  sprite.setScale(sf::Vector2f(scale, scale));
  sprite.setTexture(spritesheet);

  // default to idle state; enforce this?
  setState(IDLE, 0);
}

void Animator::draw(sf::RenderWindow &window, sf::Vector2f pos) {
  if (curr_state == NONE)
    return;

  Animation target = getAnim(curr_state, curr_anim);

  // draw rect tracks current frame; must be done continuously
  draw_rect.left = (target.coords.x + curr_frame) * target.frame_dimensions.x;
  draw_rect.top = target.coords.y * target.frame_dimensions.y;

  sprite.setTextureRect(draw_rect);
  sprite.setPosition(pos);

  // draw sprite to window
  window.draw(sprite);
}

void Animator::update(float dt) {
  stepFrame(dt);

  // debug
  std::cout << "State, Direction, Frame/Total: " << curr_state << ", "
            << curr_anim << ", " << curr_frame << "/" << curr_frame_count
            << std::endl;
}
