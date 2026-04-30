# COS10009 Custom Program
## Introduction

This is a custom project for the COS10009 course. It is a small game written in C Language. The programme also utilises SDL2 as an additional library.

This is a simple PvP mini-game featuring Player 1 and Player 2. In this game, players can move freely within a small area and control their characters to attack.
Health packs that restore health are generated randomly within the scene; players can collide with these packs to regain health.
The player who successfully defeats their opponent wins.

**Simple DirectMedia Layer (SDL)** is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D

Here is the [other resource](#other-resources) for learning.

## Project Setup

### C Language setup

C language project setup can reference here, is more details:
\
[C/C++ for Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp)

SDL is already included in this GitHub repository, so there is no need to download any additional software.

### CMake Setup

In Visual Studio Code, you can choose CMake to automate the build process.

The CMake deployment files are already included in this GitHub repository, so there is no need to set up any additional files.

If you wish to use CMake for automated builds, you may wish to refer to the configuration guide for CMake in Visual Studio Code.

[Create a CMake hello world project with CMake Quick Start](https://code.visualstudio.com/docs/cpp/cmake-quickstart)

## Launch the Program
When running this programme, you can choose to use Visual Studio Code to configure the executable file and environment in advance, or you can choose to run it manually.

---

### Method 1: Configure CMake to let the programme can run in Visual Studio Code
If we choose to run our programme in Visual Studio Code rather than manually, we need to configure Visual Studio Code.

Firstly, we need to download two extensions: the C/C++ extension and the CMake extension. The C/C++ extension is an official Microsoft extension that allows users to write, install and run C and C++ code within VS Code. As this programme is written in C, we need this extension to assist us.

The CMake extension, meanwhile, makes it easier to ‘configure, compile and manage’ CMake projects within the editor, simplifying the project’s execution.

---

### Method 2: Run the programme manually

In the terminal where you manually run this programme, you can choose to use the Mingw you just downloaded, or you can run it using Windows’ built-in PowerShell or cmd. There are differences between the two methods.

- GCC
```Shell
gcc main.c -o ./bin/main.exe -I./include -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
```

- Clang
```Shell
clang main.c -o ./bin/main.exe -I./include -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
```

Run the exe file:
```Shell
./bin/main.exe
```

# Other Resources
[C/C++ for Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp)
\
[MSYS2](https://www.msys2.org/)
\
[CMake - Upgrade Your Software](https://cmake.org/)