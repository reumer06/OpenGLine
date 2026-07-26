# OpenGLine

A comparison of 2D line rasterization algorithms (**Basic Slope-Based** vs. **Bresenham's Algorithm**) rendered in OpenGL.

### 1. Basic Algorithm

The standard approach based on the classic line equation ($y = mx + c$). It walks the primary axis step-by-step and calculates fractional positions along the secondary axis using the slope ratio $m = \frac{\Delta y}{\Delta x}$.

### 2. Bresenham's Algorithm

Uses an accumulated integer error term (`err = dx - dy`) to determine whether to take a horizontal, vertical, or diagonal pixel step at each iteration.

## Quick Start

1.**Clone the repository:**

```bash
git clone https://github.com/reumer06/OpenGLine.git
cd OpenGLine
```

2.**Configure with CMake:**

```bash
cmake -G Ninja -B build

```

3.**Build the executable:**

```bash
cmake --build build

```

4.**Run:**

```bash
./build/program

```

## License

MIT License. See `LICENSE` for details.
