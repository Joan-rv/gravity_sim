# gravity_sim
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
Use WASD to move the screen and the scroll wheel to zoom in and out. To change the density type any number <= 255 and press enter.
