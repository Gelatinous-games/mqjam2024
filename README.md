# macsjam2024

Game made for MACSJAM 2024 - Re-entry
Using C & Raylib.

Built for Windows & (later) WebGL.

The project is located under `/main/`. To run it you must have the raylib installed:
https://raysan5.itch.io/raylib

You will also want mingw32
https://www.mingw-w64.org

Open `/main/main.code-workspace/` in VSCode, and navigate to `main.c`. It can be run from here.

* itch page is [this page](https://refreshingcucumber.itch.io/light-shift)

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