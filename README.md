# FastEngine

FastEngine is a lightweight CPU-based renderer that performs rendering without external tools and uses a fork of SDL3 (make sure this code works for a while) to display the final rendered buffer.

## Clone Instructions

```bash
git clone --recursive https://github.com/AlexSid0/FastEngine.git
```

If you cloned without `--recursive`, run:

```bash
git submodule update --init
```

## Build & Run

Create a build folder and build the project using CMake:

```bash
mkdir build
cmake -B build -S .
cmake --build build
```

When running the executable, make sure to run it from the project root directory, **not from inside the build folder**, so that relative file paths work correctly.

If you have followed the steps above, running the program should be as simple as:

```bash
build/FastEngine (.exe)
```