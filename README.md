# MQJAM 2024 -- RE-ENTRY
## STABLE - branch

* this branch is only to be used for last *most* stable code.
* new features should only be added to this once they're tested.

## About:

Game made for MQJAM 2024 - Re-entry
Using C & Raylib.

Built for Windows & (later) WebGL.

The project is located under `/main/`. To run it you must have the raylib installed:
https://raysan5.itch.io/raylib

You will also want mingw32
https://www.mingw-w64.org

Open `/main/main.code-workspace/` in VSCode, and navigate to `main.c`. It can be run from here.

* itch page is [this page](https://refreshingcucumber.itch.io/light-shift)

---

## Developers:

The code you should worry about is creating objects. The data structure of an object can be found under `/main/src/base.h`, and an example on how to create a unique object is under `main/src/objs/Object_Example.h`.
If you encounter problems contact Harris.

### Important notes:
As mentioned above, the bulk of editable code is expanding on `base.h`. An example is found under `Object_Example.c`, which is a tester object I've been using.
Each object base has 4 function references which you can overwrite in a dedicated creation function for your object. These functions are for initialization, 
destruction, frame logic & rendering. Make sure to destroy all resources.

A little wrapper for sprites is found under sprite.c, with the CreateSprite function. Make sure to destroy this sprite once your object is destroyed.
This sprite object can be passed to the variants of RenderSprite to draw it at any position. This supports rotations.


* UNIX uses:
```bash
cmake -DBUILD_SHARED_LIBS=ON -B build && cmake --build build && ./build/gamer
```

---

## Planning things

###  Sound / Music

* something for asteroid impacting stars, brown noise maybe?

### Sprites

* more stars
* removing black layer from the background sprites
* asteroid variants

### Shields

* smaller particles
* matches the collision radius

### Gravity reader

* shows how much gravity is effecting your ship like a geiger counter

### Health / damage

* damage number toasts
* neaten up the health bar?

### Timer

* holds how long it took you to get to wormhole
* new counter below it for the return
* shows times in the win screen

---

## web building

* get emscripten

* needed to install ninja via brew
actually using:
```bash
cmake -S . -B buildweb -G Ninja -DPLATFORM=Web "-DCMAKE_TOOLCHAIN_FILE=<fullpath_to_emsdk>/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
```
* aurora uses:
```bash
cmake -S . -B buildweb -G Ninja -DPLATFORM=Web "-DCMAKE_TOOLCHAIN_FILE=/Users/auroragriffith/emscripten_pain/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DCMAKE_BUILD_TYPE=Release
```

then:
```bash
cmake --build buildweb
```
