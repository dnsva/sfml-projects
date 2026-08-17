# sfml-projects

A handful of small C++ programs I've built with SFML while learning graphics programming and CMake. Nothing here depends on anything else, so pick a folder, build it, run it. Each one under `src/` has its own README with more detail than what's below.

## Building

One `CMakeLists.txt` at the root handles everything and pulls SFML 2.6.x through `FetchContent`, so you don't need to install SFML yourself first.

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

That produces five binaries in `build/bin/`:

| Binary | Project |
|---|---|
| `conway` | Conway's Game of Life |
| `graphs` | Graphs |
| `triangle` | Sierpinski Triangle |
| `spiral` | Ulam's Spiral |
| `widgets` | annas_library demo |

## Conway's Game of Life

Click cells to seed a starting generation, set the grid size, then run it and watch the population change from one generation to the next. More in [src/conway](src/conway).

<video src="src/conway/example.mp4" controls width="500"></video>

## Graphs

Add and remove nodes, then connect them with directed or double-sided edges.

<img src="src/graphs/add_node.png" width="500">
<img src="src/graphs/updated_add_node.png" width="500">

<video src="src/graphs/demo.mov" controls width="500"></video>

Still working through a bug where edges don't always get cleaned up when a node is deleted. Rest of the to-do list is in [src/graphs](src/graphs).

## Sierpinski Triangle

Draws a Sierpinski triangle recursively. Default precision is 1 pixel; pass a number on the command line to change it. [src/sierpinski_triangle](src/sierpinski_triangle) has both options.

<img src="src/sierpinski_triangle/triangle.png" width="500">

## Ulam's Spiral

Plots numbers in a spiral and marks the primes, so you can see the diagonal lines Ulam noticed by hand in 1963. Grid size is also a command-line argument, covered in [src/ulam_spiral](src/ulam_spiral).

<img src="src/ulam_spiral/10x10.png" width="300">
<img src="src/ulam_spiral/100x100.png" width="300">
<img src="src/ulam_spiral/1000x1000.png" width="300">

## annas_library

A small set of reusable SFML widgets, a button and a textfield, that I'm building out to eventually share across the other projects instead of rewriting the same click-detection code every time. [src/annas_library](src/annas_library) has the source and usage examples.

<video src="src/annas_library/textfield_demo.mov" controls width="500"></video>

## Fonts

Everything that draws text pulls the same `arial.ttf` from `src/fonts` through `annas_library::get_default_font()`. The font path resolves at compile time via a CMake-supplied `FONTS_DIR`, so the binaries load it correctly no matter what directory you run them from.
