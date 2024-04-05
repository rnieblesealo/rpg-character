#ifndef STATE_H
#define STATE_H

#include <map>

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

enum State { NONE, IDLE, WALK, ATTACK };
enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct StateInfo {
public:
  sf::Vector2i coords; // row, col of first frame of animation
  int frame_count;

  StateInfo(sf::Vector2i coords, int frame_count); 
} StateInfo;

typedef std::map<Direction, StateInfo> DirectionMap;
typedef std::map<State, DirectionMap> StateMap;
typedef std::map<State, sf::SoundBuffer &>
    SoundMap; // we use reference because we need to loadFromFile

#endif
