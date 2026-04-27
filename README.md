# COS10009-Custom-Program
## Introduction

This is a custom project for the COS10009 course. It is a small game written in C. The programme also utilises SDL2 as an additional library.
\
This is a simple PvP mini-game featuring Player 1 and Player 2. In this game, players can move freely within a small area and control their characters to attack.
Health packs that restore health are generated randomly within the scene; players can collide with these packs to regain health.
The player who successfully defeats their opponent wins.
\
Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D

## Project Setup
Before running this programme, we need to download the relevant configuration file


## Launch the Program
When running this programme, you can choose to use Visual Studio Code to configure the executable file and environment in advance, or you can choose to run it manually.

In the terminal where you manually run this programme, you can choose to use the Mingw you just downloaded, 
or you can run it using Windows’ built-in PowerShell or cmd. There are differences between the two methods.
```
gcc main.c -o main.exe -lmingw32 -lsdl2main -lsdl2 -lsdl2_ttf -lsdl2_image -lsdl2_mixer
./main.exe
```