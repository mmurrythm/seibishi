# SEIBISHI

## Requirements

* **C compiler:** GCC/MinGW
* **raylib:** Required for graphics, input, audio, textures, and window management.
* Standard C library and the libraries required by raylib.

## Project Structure

```text
SEIBISHI/
├── seibishi.c
└── assets/
    ├── audio/
    ├── texture/
    └── logfiles/
```

The asset paths used by the program are relative paths, so the directory structure and filenames must remain unchanged.

## Dependencies and Setup

### 1. Install GCC/MinGW

Install a GCC-based C compiler that can build raylib programs.

Verify the compiler is available:

```bash
gcc --version
```

### 2. Install raylib

Install/configure raylib so that GCC can locate its headers and library files.

The source includes raylib with:

```c
#include "raylib.h"
```

Therefore, the raylib development files must be available to the compiler and linker.

## Compilation

Open a terminal in the **SEIBISHI project root**, where `seibishi.c` and the `assets` directory are located.

For a MinGW/GCC environment, compile with:

```bash
gcc seibishi.c -o seibishi.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

A successful compilation should produce:

```text
seibishi.exe
```

## Running the Game

Run the executable **from the project root directory** so that all relative asset paths remain valid:

```bash
.\seibishi.exe
```

Do not move the executable away from the project directory unless the required asset paths are also preserved.

## Required Assets

The game requires the existing files inside:

```text
assets/audio/
assets/texture/
assets/logfiles/
```

The program loads its textures and audio files at runtime. Missing, renamed, or relocated assets can prevent the game from working correctly.

## Persistent Files

The game uses files under:

```text
assets/logfiles/
```

for persistent game data, including:

```text
save.txt
leaderboard.txt
profile.txt
settings.txt
```

These files should remain accessible to the executable. Do not delete or relocate them if existing saved progress, leaderboard data, profile information, or settings need to be preserved.

## Troubleshooting

### `raylib.h: No such file or directory`

raylib is not installed or its include path is not configured correctly.

### `undefined reference` errors involving raylib

The raylib library or required linker libraries are not being linked correctly. Check the compilation command and raylib installation.

### Textures or sounds fail to load

Make sure the program is being executed from the project root and that the `assets` directory has not been moved or renamed.

### Save, leaderboard, profile, or settings data is not preserved

Check that the program has permission to read and write files in:

```text
assets/logfiles/
```

## Quick Setup

From a correctly configured GCC/raylib environment:

```bash
cd SEIBISHI
gcc seibishi.c -o seibishi.exe -lraylib -lopengl32 -lgdi32 -lwinmm
.\seibishi.exe
```
