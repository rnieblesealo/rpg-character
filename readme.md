### Checklist

- [ ] Get player instance working

### Sprite Data

- Passed in sprites should be an image laid out as a grid with evenly sized tiles
- What we pass into the player class:
    - **Vector2i**: (X, Y) of animation start, relative to tile size and not pixels
    - **enum State**: Action represented by animation (attacking, moving, etc...)
    - **enum Direction**: 4-directional variation of animation
    - **int**: Frame count of animation
- Store them as a separate class?
