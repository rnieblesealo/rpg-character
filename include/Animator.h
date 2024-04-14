#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <map>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

enum State { NONE, IDLE, WALK, ATTACK };

class Animation {
public:
  sf::Vector2i coords; // row, col of first frame of animation
  sf::Vector2i frame_dimensions;
  int frame_count;

  Animation(sf::Vector2i coords, sf::Vector2i frame_dimensions,
            int frame_count);
};

class SplitAnimation : public Animation {
  sf::Texture &spritesheet;

  SplitAnimation(sf::Texture &spritesheet, sf::Vector2i coords,
                 sf::Vector2i frame_dimensions, int frame_count);
};

typedef std::map<State, std::vector<Animation>> AnimationList;
typedef std::map<State, sf::SoundBuffer &> SoundMap;

class Animator {
  sf::Sprite sprite;
  sf::IntRect draw_rect{0, 0, 0, 0};
  float scale{2};

  int fps{8};
  int curr_frame_count{0};
  int curr_frame{0};
  float frame_timer{0};

  AnimationList anims;

  State curr_state{NONE};
  bool in_action{false};
  int curr_anim{0};
  int default_anim{0}; // default animation of every state

  SoundMap sfx;
  sf::Sound sound;

  void stepFrame(float dt);

  Animation &getAnim(State state, int idx);

public:
  Animator(sf::Texture &spritesheet, AnimationList &anims, SoundMap &sfx);

  void startAction(State action, int anim = -1);
  void endAction();
  void playSound(State source);
  void setState(State new_state, int idx = -1, bool reset = false);
  void setAnim(int idx);
  void draw(sf::RenderWindow &window, sf::Vector2f pos);
  void update(float dt);
};

#endif
