#include <filesystem>
#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

enum State {
  NONE,

  IDLE_UP,
  IDLE_DOWN,
  IDLE_L,
  IDLE_R,

  WALK_UP,
  WALK_DOWN,
  WALK_L,
  WALK_R,

  ATTACK_UP,
  ATTACK_DOWN,
  ATTACK_L,
  ATTACK_R
};

typedef struct AnimInfo {
  AnimInfo(sf::Vector2i coords, int frame_count) {
    this->coords = coords;
    this->frame_count = frame_count;
  }

  sf::Vector2i coords; // row, col of first frame of animation
  int frame_count;

} AnimInfo;

class Player {
private:
  sf::Vector2f pos{0, 0};
  sf::Vector2f vel{0, 0};

  float speed{150};
  float scale{1};

  sf::Sprite sprite;
  sf::IntRect draw_rect{0, 0, 0, 0};
  sf::Vector2i frame_dimensions{0, 0};

  int fps{8};
  int curr_frame_count{0};
  int curr_frame{0};
  float frame_timer{0};

  std::map<State, AnimInfo> &anims;
  State curr_state{NONE};

  void onUserInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      vel.y = -1;
      setState(WALK_UP);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      vel.y = 1;
      setState(WALK_DOWN);
    }

    else
      vel.y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      vel.x = -1;
      setState(WALK_L);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      vel.x = 1;
      setState(WALK_R);
    }

    else
      vel.x = 0;
  }

  void move(float dt) {
    pos += vel * speed * dt;
    sprite.setPosition(pos);
  }

  void stepFrame(float dt) {
    if (curr_state == NONE)
      return;

    frame_timer += dt;

    // move frames, if reach last frame, go back to first one
    if (frame_timer >= 1.0f / fps) {
      if (++curr_frame == curr_frame_count)
        curr_frame = 0;
      frame_timer = 0;
    }
  }

  void setState(State new_state) {
    if (new_state == curr_state)
      return;

    // map.find() returns map.end() if value not found for given key
    if (anims.find(new_state) == anims.end()) {
      curr_state = NONE;
      return;
    }

    curr_state = new_state;

    AnimInfo target = anims.at(curr_state);

    curr_frame_count = target.frame_count;
  }

  void animate() {
    if (curr_state == NONE)
      return;

    AnimInfo target = anims.at(curr_state);

    // draw rect tracks current frame
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
         std::map<State, AnimInfo> &anims)
      : anims(anims) {
    this->frame_dimensions = frame_dimensions;
    this->anims = anims;

    draw_rect.width = frame_dimensions.x;
    draw_rect.height = frame_dimensions.y;

    sprite.setTexture(spritesheet);
  }

  void draw(sf::RenderWindow &window) {
    // draw sprite to window
    window.draw(sprite);
  }

  void debug_info(){
    std::cout << "----------" << std::endl;
    std::cout << "STATE: " << curr_state << std::endl;
    std::cout << "DRAW RECT: " << draw_rect.top << ", " << draw_rect.left << ", " << draw_rect.width << ", " << draw_rect.height << std::endl;
    std::cout << "CURRENT FRAME/CURR FRAME COUNT: " << curr_frame << "/" << curr_frame_count <<  std::endl;
    std::cout << "----------" << std::endl;
  }

  void update(float dt) {
    onUserInput();

    animate();
    move(dt);
    stepFrame(dt);
    
    // debug_info
  }
};

int main() {
  std::string cwd = std::filesystem::current_path().generic_string();

  sf::Texture player_tex;
  if (!player_tex.loadFromFile(cwd + "/assets/player.png"))
    return EXIT_FAILURE;

  std::map<State, AnimInfo> player_anims = {
      {WALK_DOWN, AnimInfo(sf::Vector2i(0, 0), 3)},
      {WALK_R, AnimInfo(sf::Vector2i(0, 1), 3)},
      {WALK_L, AnimInfo(sf::Vector2i(0, 2), 3)},
      {WALK_UP, AnimInfo(sf::Vector2i(0, 3), 3)},
      {ATTACK_DOWN, AnimInfo(sf::Vector2i(3, 0), 3)},
      {ATTACK_R, AnimInfo(sf::Vector2i(3, 1), 3)},
      {ATTACK_L, AnimInfo(sf::Vector2i(3, 2), 3)},
      {ATTACK_UP, AnimInfo(sf::Vector2i(3, 3), 3)},
  };

  sf::VideoMode vm(800, 600);
  sf::RenderWindow window(vm, "RPG");

  window.setFramerateLimit(60);

  sf::Clock dt_clock;
  float dt;

  Player player(player_tex, sf::Vector2i(32, 32), player_anims);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    dt = dt_clock.restart().asSeconds();
    
    std::cout << "HELLO" << std::endl;

    player.update(dt);

    window.clear(sf::Color::Black);

    player.draw(window);

    window.display();
  }

  return 0;
}
