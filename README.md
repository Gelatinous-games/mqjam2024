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

---
---

## Developers:

> [!TIP]
> The code you should worry about is creating objects. The data structure of an object can be found under `/main/src/base.h`, and an example on how to create a unique object is under `main/src/objs/Object_Example.h`.

> [!NOTE]
> If you encounter problems contact Harris.


---

### Important notes:

> [!IMPORTANT]
> As mentioned above, the bulk of editable code is expanding on `base.h`. An example is found under `Object_Example.c`, which is a tester object I've been using.
> 
> Each object base has 4 function references which you can overwrite in a dedicated creation function for your object. These functions are for initialization, 
destruction, frame logic & rendering.

> [!CAUTION]
> Make sure to destroy all resources

> [!WARNING]
> A little wrapper for sprites is found under sprite.c, with the CreateSprite function. Make sure to destroy this sprite once your object is destroyed.

> [!TIP]
> This sprite object can be passed to the variants of RenderSprite to draw it at any position. This supports rotations.

---

### Building

* either use the `Makefile` or the `CMakeLists.txt`

#### Unix users - CMAKE

> [!TIP]
> need to open `${workspaceFolder}/main` in terminal then copy paste this in: 
```bash
cmake -DBUILD_SHARED_LIBS=ON -B build && cmake --build build && ./build/gamer
```

#### Windows users - CMAKE

> [!WARNING]
> idk if it'll work tho

> [!TIP]
> need to open `${workspaceFolder}/main` in terminal then copy paste this in: 
```bash
cmake -DBUILD_SHARED_LIBS=ON -B build
cmake --build build
./build/gamer
```

---
---

## Planning things

###  Sound / Music

* idk, u guys get free reign, just let us know if it's meant to loop or just play once
    * *if you want, an approaching wormhole/planet loop would be nice*
* you guys have done amazing!

#### === Music usage

* looks like `.mp3` exists
    * [here is example code](https://www.raylib.com/examples/audio/loader.html?name=audio_music_stream)

---

### Features/workables

#### === Planet/Wormhole approaching ===

* needs a thing for handling when we approach the planet or the wormhole
    * if immediately go back to planet, maybe that's shop or something
* refering to the planets / wormholes as "*interactable*"
* grow when departing *interactable*
* shrink when approaching *interactable*

#### === Health ===

* need a health manager
* health manager can say states?


#### === Sprite layering ===

* needs something to handle multi-layer sprites so we can transform individual layers
* can just be an array
* probably also handles tinting layers

#### === Sprite managing ===

* ***SCOPE CREEP***
* should all sprites be in a manager as already loaded then we ask by ID?
* KV table or something? makes it less painful to add new ones

#### === Effects ===

* ***SCOPE CREEP***
* scripts for handling specific particle scenarios
* explosion / trail / impact / debris / forcefield
* just an object for manipulating particles specific to the scenario
* strategy pattern kinda vibe so we can just add the effect to anything and reuse

---
---

## CURRENT WORKINGS

> [!NOTE]
> this is where aurora is keeping tabs on sunday workings

---

### Things to worry about

- [x] scream
- [x] need to standardise the creating stars/sprites for background
- [ ] need to mess with star movement vectors so they look normal
- [x] double player? how?
- [x] destroy things and rebuild when die?
- [x] need to change deathmanager destroy function to create new game manager
- [x] may need to fix up the ASTEROID_REF_LIST free in main.c
- [x] restartManager.c renamed
- [ ] make the win menu
- [ ] death menu overlay
- [ ] ...
- [ ] ...


---