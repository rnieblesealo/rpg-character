These are just some loose notes to keep track of how some things work for now!

### Checklist

- [x] Get player instance working
- [x] Attacking/actions
- [x] **Error handling!**
- [ ] Health bar/GUI

### Just 'Cuz

- [x] Sound

---

### Sprite Data

- Passed in spritesheet images should have **evenly-sized frames** laid out in **horizontal** sequence

### Player Class

### Args

- **Spritesheet**: Texture ref. to spritesheet
- **Frame Dimensions**: Pixel dimensions of one frame of an animation 
- **StateMap**: Contains animation data 

### Types 

- `DirectionMap`: Maps a `Direction` to an animation
- `StateMap`: Maps a `State` to a DirectionMap 

### Variables

- `in_action`: Disallows `setState()` control and yields it once passed `action`'s animation ends
