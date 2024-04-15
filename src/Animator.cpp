#include "Animator.h"

Animation::Animation(sf::Vector2i coords, sf::Vector2i frame_dimensions,
                     int frame_count) {
  this->coords = coords;
  this->frame_dimensions = frame_dimensions;
  this->frame_count = frame_count;
}

void Animator::stepFrame(float dt) {
  frame_timer += dt;

  if (frame_timer >= 1.0f / fps) {
    if (++curr_frame == curr_frame_count) {
      curr_frame = 0;
    }

    frame_timer = 0;
  }
}

void Animator::playSound(int sound) {
  audio_source.setBuffer(sfx.at(sound));
  audio_source.play();
}

void Animator::playAnim(int anim) {
  curr_anim = anim;

  Animation target = anims.at(anim);

  draw_rect.width = target.frame_dimensions.x;
  draw_rect.height = target.frame_dimensions.y;
  curr_frame_count = target.frame_count;
  curr_frame = 0;
}

Animator::Animator(sf::Texture &spritesheet,
                   std::array<Animation, MAX_ANIMS> &anims,
                   std::array<sf::SoundBuffer, MAX_SOUNDS> &sfx)
    : anims(anims), sfx(sfx) {
  this->anims = anims;
  this->sfx = sfx;

  sprite.setScale(sf::Vector2f(scale, scale));
  sprite.setTexture(spritesheet);
}

void Animator::draw(sf::RenderWindow &window, sf::Vector2f pos) {
  Animation target = anims.at(curr_anim); 

  // draw rect tracks current frame; must be done continuously
  draw_rect.left = (target.coords.x + curr_frame) * target.frame_dimensions.x;
  draw_rect.top = target.coords.y * target.frame_dimensions.y;

  sprite.setTextureRect(draw_rect);
  sprite.setPosition(pos);

  window.draw(sprite);
}

void Animator::update(float dt) {
  stepFrame(dt);
}
