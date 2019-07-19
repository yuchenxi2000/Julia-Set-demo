# Julia Set demo

a simple julia set demo written in c++ (OpenGL)

You can download Mac App from releases. (v1.0.0)

## Library Used

1. glew
2. glfw
3. glm

## Feature

This program uses shader program to calculate the image. Faster than calculate using CPU, but accuracy may be lower.

## How to Use

* press W/A/S/D to move the image up/down/left/right
* press +/- to zoom in/out
* press J/K/N/M to move the point C in the complex plane (change the shape of julia set)

> Point C : the parameter in formula z\_n = z\_(n-1) * z\_(n-1) + C

