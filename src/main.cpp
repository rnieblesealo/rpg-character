#include <filesystem>
#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

enum State { NONE, IDLE, WALK, ATTACK };
enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct StateInfo {
public:
  sf::Vector2i coords; // row, col of first frame of animation
  int frame_count;

  StateInfo(sf::Vector2i coords, int frame_count) {
    this->coords = coords;
    this->frame_count = frame_count;
  }
} StateInfo;

typedef struct std::map<Direction, StateInfo> DirectionMap;
typedef struct std::map<State, DirectionMap> StateMap;

class Player {
private:
  sf::Vector2f pos{0, 0};
  sf::Vector2i vel{0, 0};

  float base_speed{175};
  float speed_multiplier{1};
  std::map<State, float> speed_multipliers = {{ATTACK, 0.5}};

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

  void onUserInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
      startAction(ATTACK);
    }

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
  }

  void move(float dt) {
    pos += static_cast<sf::Vector2f>(vel) * base_speed * speed_multiplier * dt;
    sprite.setPosition(pos);
  }

  void stepFrame(float dt) {
    if (curr_state == NONE)
      return;

    frame_timer += dt;

    // move frames, if reach last frame, go back to first one
    if (frame_timer >= 1.0f / fps) {
      if (++curr_frame == curr_frame_count) {
        if (in_action)
          endAction();
        curr_frame = 0;
      }

      frame_timer = 0;
    }
  }

  void startAction(State action) {
    setState(action);
    in_action = true;
  }

  void endAction() { in_action = false; }

  void setState(State new_state) {
    if (curr_state == new_state || in_action)
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

  void setDir(Direction new_dir) {
    if (curr_dir == new_dir) {
      return;
    }

    curr_dir = new_dir;

    // actions are one-time animations; we should never reset them
    if (!in_action)
      curr_frame = 0;
  }

  DirectionMap &getDirectionMap(State state) { return states.at(state); }

  StateInfo &getStateInfo(State state, Direction dir) {
    return states.at(state).at(dir);
  }

  void animate() {
    if (curr_state == NONE)
      return;

    StateInfo target = getStateInfo(curr_state, curr_dir);

    // draw rect tracks current frame; must be done continuously
    draw_rect.left = (target.coords.x + curr_frame) * frame_dimensions.x;
    draw_rect.top = target.coords.y * frame_dimensions.y;

    sprite.setTextureRect(draw_rect);
  }

public:
  /* why constructor initializer here?
   * why for map but not for texture? is it because map cannot be default
   * constructed? but how is a reference default constructed?
   */
  Player(sf::Texture &spritesheet, sf::Vector2i frame_dimensions,
         StateMap &states)
      : states(states) {
    this->frame_dimensions = frame_dimensions;
    this->states = states;

    draw_rect.width = frame_dimensions.x;
    draw_rect.height = frame_dimensions.y;

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setTexture(spritesheet);
  }

  void draw(sf::RenderWindow &window) {
    // draw sprite to window
    window.draw(sprite);
  }

  void update(float dt) {
    onUserInput();

    animate();
    move(dt);
    stepFrame(dt);

    // debug
    std::cout << "State, Direction, Frame/Total: " << curr_state << ", "
              << curr_dir << ", " << curr_frame << "/" << curr_frame_count
              << std::endl;
  }
};

int main() {
  std::string cwd = std::filesystem::current_path().generic_string();

  sf::Texture player_tex;
  if (!player_tex.loadFromFile(cwd + "/assets/player.png"))
    return EXIT_FAILURE;

  // player animation data loading is extremely clunky; find an automated way to
  // do this using files
  DirectionMap player_idle = {
      {UP, StateInfo(sf::Vector2i(1, 3), 1)},
      {DOWN, StateInfo(sf::Vector2i(1, 0), 1)},
      {LEFT, StateInfo(sf::Vector2i(1, 2), 1)},
      {RIGHT, StateInfo(sf::Vector2i(1, 1), 1)},
  };

  DirectionMap player_walk = {
      {UP, StateInfo(sf::Vector2i(0, 3), 3)},
      {DOWN, StateInfo(sf::Vector2i(0, 0), 3)},
      {LEFT, StateInfo(sf::Vector2i(0, 2), 3)},
      {RIGHT, StateInfo(sf::Vector2i(0, 1), 3)},
  };

  DirectionMap player_attack = {
      {UP, StateInfo(sf::Vector2i(3, 3), 3)},
      {DOWN, StateInfo(sf::Vector2i(3, 0), 3)},
      {LEFT, StateInfo(sf::Vector2i(3, 2), 3)},
      {RIGHT, StateInfo(sf::Vector2i(3, 1), 3)},
  };

  StateMap player_states = {
      {IDLE, player_idle}, {WALK, player_walk}, {ATTACK, player_attack}};

  sf::VideoMode vm(800, 600);
  sf::RenderWindow window(vm, "RPG");

  window.setFramerateLimit(60);

  sf::Clock dt_clock;
  float dt;

  Player player(player_tex, sf::Vector2i(32, 32), player_states);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    dt = dt_clock.restart().asSeconds();

    player.update(dt);

    window.clear(sf::Color::Black);

    player.draw(window);

    window.display();
  }

  return 0;
}
