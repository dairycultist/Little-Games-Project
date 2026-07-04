# Little Games Project
A fantasy spec; simulates the fun of developing for restricted hardware with the freedom to use any language for any hardware.

For a game to fit the LGP spec:
- It must render to a window of 256x192 pixels
- It must use as inputs a D-pad (up, down, left, right) and at most 4 action keys; the hardware bindings are left up to you (e.g. you may choose to map the D-pad inputs to WASD)
- You must publish source!
- The source must be below 1mb uncompressed and uncompiled. The source is what you're gonna be transferring to players, and not all languages compile anyways. If your game loads compressed assets during runtime, that's OK; the end-user just shouldn't have to decompressed anything manually before compiling/running your game
- It must not use hardware rendering (e.g. OpenGL, Vulkan, or any other GPU API) 

## Frameworks

Planned: C, Node.js, Java
