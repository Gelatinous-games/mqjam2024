# MQJAM 2024 -- RE-ENTRY

## About:

Game made for MQJAM 2024 - Re-entry
Using C & Raylib.

Built for Windows & (later) WebGL.

To run the project you must have the raylib installed:
https://raysan5.itch.io/raylib

You will also want mingw32
https://www.mingw-w64.org

Open `./main.code-workspace` in VSCode, and navigate to `./main.c`. It can be run from here.

* itch page is [this page](https://refreshingcucumber.itch.io/light-shift)
* live page is [this page](https://gelatinous.games/mqjam2024)

---

## Developers:

The code you should worry about is creating objects. The data structure of an object can be found under `./src/base.h`, and an example on how to create a unique object is under `./src/objs/Object_Example.h`.
If you encounter problems contact Harris.

### Important notes:
As mentioned above, the bulk of editable code is expanding on `base.h`. An example is found under `Object_Example.c`, which is a tester object I've been using.
Each object base has 4 function references which you can overwrite in a dedicated creation function for your object. These functions are for initialization, 
destruction, frame logic & rendering. Make sure to destroy all resources.

A little wrapper for sprites is found under `sprite.c`, with the `CreateSprite` function. Make sure to destroy this sprite once your object is destroyed.
This sprite object can be passed to the variants of `RenderSprite` to draw it at any position. This supports rotations.


### UNIX building

* if you wanna use the terminal, open the workspace folder, then use:

```bash
cmake -DBUILD_SHARED_LIBS=ON -B build && cmake --build build && ./build/gamer
```

* but if you get the cmake extensions for vscode
  * then open `./main.code-workspace`
  * you can just use the little buttons in the bottom strip for setting up cmake
  * wrench and screwdriver then select a build
    * aurora uses the clang one
  * build it first with the cog
  * then debug it with the bug

---

### web building

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