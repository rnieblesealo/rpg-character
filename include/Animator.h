#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <array>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define NONE -1
#define MAX_ANIMS 32
#define MAX_SOUNDS 32

class Animation {
public:
  sf::Vector2i coords; // row, col of first frame of animation
  sf::Vector2i frame_dimensions;
  int frame_count;

  Animation(sf::Vector2i coords, sf::Vector2i frame_dimensions,
            int frame_count);
};

class Animator {
  std::array<Animation, MAX_ANIMS>& anims;
  std::array<sf::SoundBuffer, MAX_SOUNDS>& sfx;

  sf::Sprite sprite;
  sf::IntRect draw_rect{0, 0, 0, 0};
  float scale{2};

  int fps{8};
  int curr_frame_count{0};
  int curr_frame{0};
  int curr_anim{0};
  float frame_timer{0};

  sf::Sound audio_source;

  void stepFrame(float dt);

public:
  Animator(sf::Texture &spritesheet, std::array<Animation, MAX_ANIMS>& anims, std::array<sf::SoundBuffer, MAX_SOUNDS>& sfx);

  void playSound(int sound);
  void playAnim(int anim);
  void draw(sf::RenderWindow &window, sf::Vector2f pos);
  void update(float dt);
};

#endif
