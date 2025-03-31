# Tanks

![sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=alegemaate_Tanks&metric=code_smells)
![sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=alegemaate_Tanks&metric=bugs)

A 2D, top down, tank fighting game built using SDL2

![image](https://user-images.githubusercontent.com/9920336/140231515-f7908323-4cda-44f5-91cb-4cbe48ccdb20.png)

## Setup

### Dependencies

Dependencies are managed by CMake.

### Windows (MSYS2)

```bash
pacman -S mingw-w64-i686-gcc-libs mingw-w64-i686-SDL2 mingw-w64-i686-SDL2_mixer mingw-w64-i686-SDL2_image mingw-w64-i686-SDL2_ttf
```

### Mac OS

```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### Linux

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Build

```bash
cmake .
make
```

### Build Emscripten

```bash
emcmake cmake .
make
```
