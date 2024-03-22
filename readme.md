### Checklist

- [x] Get player instance working
- [ ] Attacking

---

### Docs (?)

These are just some loose notes to keep track of how some things work for now!

##### Sprite Data

- Passed in spritesheet images should have **evenly-sized frames** laid out in **horizontal** sequence

##### Player Class

- Args:
    - **Spritesheet**: Texture ref. to spritesheet
    - **Frame Dimensions**: Pixel dimensions of one frame of an animation 
    - **Anims**: `State : AnimInfo` map w/animation data for every state 
