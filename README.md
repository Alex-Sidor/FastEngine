# FastEngine

FastEngine is a lightweight CPU-based renderer that performs rendering without external tools and uses a fork of SDL3 (make sure this code works for a while) to display the final rendered buffer.

## Clone Instructions

```bash
git clone https://github.com/Alex-Sidor/FastEngine.git
```

## Build & Run

Create a build folder and build the project using CMake:

```bash
mkdir build
cmake -B build # -G "your generator"
cmake --build build
```

When running the executable, make sure to run it from the project root directory, **not from inside the build folder**, so that relative file paths work correctly.

If you have followed the steps above, running the program should be as simple as:

```bash
build/FastEngine (.exe)
```

TODO:
* Add model importing
* Add texture support
* Add shader support
* Finish rotation class
* Add parented rotation
* Add Lighting
