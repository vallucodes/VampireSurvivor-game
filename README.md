# Supercell Coding Competition

This was Supercell coding test. There was given basic game built with SFML library and it had bugs.

Idea of the project was to fix the bugs and add own features to the game to make it more playable and fun.

<div style="display: flex; gap: 20px;">
  <div>
    <h3>Original game</h3>
    <img src="gif/original.gif" width="550"/>
  </div>
  <div>
    <h3>My improved game</h3>
    <img src="gif/gameplay.gif" width="550"/>
  </div>
</div>

## My contribution

- Collectibles
- Player healthbar
- All projectiles
- Rotating weapon to shoot projectiles
- Collectables that can be shot as shotgun projectiles

## How to play

Download repository
``` Bash
git clone https://github.com/vallucodes/VampireSurvivor-game.git
cd VampireSurvivor-game
```

Compile the project

``` Bash
cmake -B build
cmake --build build
./build/bin/survive
```
This builds the executable in `./build/bin/`.

The exact location of the executable may differ depending on your system and compiler. Example locations are `./build/bin/survive` and `./build/bin/Debug/survive.exe`.

## Controls

- `W,A,S,D` to move
- `Mouse` to aim
- `mouse1` to shoot normal projectiles
- `mouse2` to shoot collected projectiles

## Prerequisites

You must have [CMake](https://cmake.org/) version 3.22.1 or later installed to build this project.

If you are on Linux, make sure to install SFML's dependencies. For Debian-based distros:
```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

