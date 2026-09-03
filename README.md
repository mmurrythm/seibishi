# SEIBISHI

**SEIBISHI** is a 10-level sci-fi Sokoban puzzle game written in C using raylib.

The player moves around a 16×16 grid, pushes boxes, and tries to place every box on a goal.

The project combines classic Sokoban gameplay with a custom sci-fi visual style and hand-made pixel-art textures.

## Features

- 10 Sokoban level layouts
- 16×16 tile-based boards
- WASD movement
- Arrow-key movement
- Box-pushing mechanics
- Goal detection
- Boxes-on-goals
- Level reset
- Main menu
- Instructions screen
- Level progression
- Move counter
- Goal counter
- Level counter
- Custom pixel-art graphics
- Level-complete screen
- Game-over screen

## Controls

| Key | Action |
|---|---|
| `W` / `A` / `S` / `D` | Move player |
| Arrow Keys | Move player |
| `R` | Reset current level |
| `Enter` | Start / continue to next level |
| `I` | Open instructions |
| `M` | Return to main menu |
| `Esc` | Quit from main menu |

## Project Structure

```text
SEIBISHI/
│
├── seibishi.c
├── README.md
│
└── texture/
    ├── floor.png
    ├── wall.png
    ├── goal.png
    ├── box.png
    ├── box_goal.png
    ├── player.png
    ├── menubg.png
    ├── instructions.png
    ├── win.png
    └── gameover.png
