# gravity_sim
## Build and run
A simple physics gravity simulation using C++ and SFML.

Build using CMake with:
```sh
cmake -B build
cmake --build build
```
The Boost and SFML libraries are required as dependencies. If SFML can't be found it will be built from source.\
Run with:
```sh
./build/gravity_sim
```
Use WASD to move the screen and the scroll wheel to zoom in and out. To change the density, type any number <= 255 and press enter. To add a new planet, click anywhere on the screen. Keeping the mouse held will increase its size, and moving it will show an arrow representing the planet's initial velocity.

## Technical specifications
The Boost library is only used to determine the executable location and load the font file relative to it. SFML handles all the graphics and most of the math, although some convenience functions are provided in ```math.hpp```. The physics simulation uses [Euler integration](https://en.wikipedia.org/wiki/Euler_method) to compute the velocity and position of each planet, with fixed time steps of 0.01. Each planet's acceleration is determined with [Newton's second law of motion](https://en.wikipedia.org/wiki/Newton%27s_laws_of_motion#Second_law) and [Newton's equation for universal gravitation](https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation). Because all planets are circles, determining if there is a collision is easy. To solve collisions, a constraints solver is used, with 10 iterations. This properly handles collisions with 3 or more entities (which is not true for simpler methods like using momentum).
