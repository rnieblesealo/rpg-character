#ifndef PLAYER_H
#define PLAYER_H

#include "Animator.h"

typedef enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
} Direction;

class Player {
private:
  Animator& animator;
  
  sf::Vector2f pos{0, 0};
  sf::Vector2i vel{0, 0};

  float base_speed{175};
  float speed_multiplier{1};

  std::map<State, float> speed_multipliers = {{ATTACK, 0.75}};

  void onKeypressInput(float dt);

public:
  Player(Animator& animator);

  void onEvent(sf::Event event); 
  void draw(sf::RenderWindow &window); 
  void update(float dt); 
};

#endif
