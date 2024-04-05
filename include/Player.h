#ifndef PLAYER_H
#define PLAYER_H

#include "State.h"

#include <SFML/Graphics.hpp>

class Player {
private:
  sf::Vector2f pos{0, 0};
  sf::Vector2i vel{0, 0};

  float base_speed{175};
  float speed_multiplier{1};

  std::map<State, float> speed_multipliers = {{ATTACK, 0.75}};

  float scale{2};

  sf::Sprite sprite;
  sf::IntRect draw_rect{0, 0, 0, 0};
  sf::Vector2i frame_dimensions{0, 0};

  int fps{8};
  int curr_frame_count{0};
  int curr_frame{0};
  float frame_timer{0};

  StateMap states;

  State curr_state{NONE};
  Direction curr_dir{UP};

  bool in_action = false;

  SoundMap sfx;
  sf::Sound sound;

  void onKeypressInput(float dt);
  void stepFrame(float dt);
  void startAction(State action);
  void endAction();
  void playSound(State source);
  void setState(State new_state, bool reset = false);
  void setDir(Direction new_dir);
  void animate();

  DirectionMap &getDirectionMap(State state);
  StateInfo &getStateInfo(State state, Direction dir);

public:
  /* why constructor initializer here?
   * why for map but not for texture? is it because map cannot be default
   * constructed? but how is a reference default constructed?
   */
  Player(sf::Texture &spritesheet, sf::Vector2i frame_dimensions,
         StateMap &states, SoundMap &sfx);

  void onEvent(sf::Event event); 
  void draw(sf::RenderWindow &window); 
  void update(float dt); 
};

#endif
