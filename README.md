# macsjam2024

Game made for MACSJAM 2024 - Re-entry
Using C & Raylib.

Built for Windows & (later) WebGL.

The project is located under `/main/`. To run it you must have the raylib installed:
https://raysan5.itch.io/raylib

Open `/main/main.code-workspace/` in VSCode, and navigate to `main.c`. It can be run from here.

## Developers:

The code you should worry about is creating objects. The data structure of an object can be found under `/main/src/base.c`, and an example on how to create a unique object is under `main/src/objs/Object_Example.c`.
If you encounter problems contact Harris.




```bash
cmake -B build && cmake --build build && ./build/gamer
```