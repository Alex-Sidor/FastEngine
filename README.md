FastEngine is lightweight CPU-based renderer that does the rendering without any additional tools then uses SDL3 to display the final rendered buffer.

Build Instructions:

```git clone --recursive https://github.com/AlexSid0/FastEngine.git```

* If this was not done previously use ```git submodule update --init```

cd FastEngine

For Visual Studio:
```cmake -B build -S .```
```cmake --build build```

For Make file on windows:
```cmake -B build -S . -G "MinGW Makefiles"```
```cmake --build build```
```.\build\FastEngine.exe```