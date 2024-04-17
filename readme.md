### Checklist

- [x] Get player instance working
- [x] Attacking/actions
- [x] **Error handling!**
- [ ] Health bar/GUI

### Just 'Cuz

- [x] Sound

## What is this going to be?

A simple game like the first Zelda, but punk rock. There's going to be a series of interconnected levels with various enemies and a boss at the end.

It's a C++ learning excercise and a game design exercise. The code doesn't have to be super pretty/scalable, but it should work and be extendable within the game's purpose!

## Game Design Spec

### Gameplay

You press one button to attack. There's going to be a melee and ranged weapon.

- **Melee**: Short-range, higher damage attack; unlimited 
- **Ranged**: Long-range, lower damage attack; ammo-based 

All enemies will shoot some sort of projectile at you. There will be obstacles in the level, with little space to move around, like a kind of maze. *The player will need to strategically move in order to avoid enemy projectiles*.

### Story/Lore

- **Setting**: 1980's suburban america, think "Heathers"
- **Theme**: Punk rock/manson
    - You will have your guitar as your only weapon, and you can either smack people with it (melee) or melt their faces with a projectile (ranged)
    - There will be one weapon upgrade before the boss. You must beat a secret level to get to it.
- **Story**: You are the lead guitarist in a local punk band. You are late to your highest-stakes show. A bunch of manson-like cultists stormed the area in the meantime, and are holding your bandmates captive at the venue. You must rescue them and kill their boss before it's too late!

## Technical Spec

These are just some loose notes to keep track of how some things work for now!

### Caveats

These are some decisions that might seem strange but have reasons behind them.

Each one comes with a **problem**, an **explanation** of the problem, a chosen **workaround**, and a **justification** for this workaround.

- **Problem**: Cannot assign to `Player` member `Animator&` (which is a reference) from the constructor 
    - **Explanation**: From my understanding, once references are set, the `=` operator gets deleted because references can't be reassigned
        - Using move semantics, we can modify the operator and change this behavior
            - This is probably why we *can* `=` references of stuff in the `sf` and `std` namespaces
    - **Workaround**: Make the `Animator` member a pointer instead of a reference
        - **Justification**: Move semantics seems like a lengthy topic I'm not familiar with. I do understand pointers, and they get the job done. 

### Sprite Data

- Passed in spritesheet images should have **evenly-sized frames** laid out in **horizontal** sequence

### `class Animator`

- Handles anything that's supposed to be animated
- **Works with an array**, where each index represents a different animation
- Each class that uses the animator may define an enum within its header file that determines which index maps to what

### `class Animation`

- Is what the array in the `Animator` holds
- Tells us information about a **single** animation 
    - An animation is a sequence of sprites
- **All animations must be located within a single spritesheet/image!**
    - It's much easier to stitch images together in a photo editor than to account for the many different ways spritesheets can be packed
- Because the slot at which an animation/sound is stored is significant, we will use static arrays and not dynamic ones
