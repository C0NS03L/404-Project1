# ICCS404-Project

## Overview

This project is a computer graphics application written in C++ that generates and displays a fantasy landscape using OpenGL. It includes features such as terrain generation, camera movement, and lighting.

## Features

- **Terrain Generation**: Uses Perlin noise to generate a 3D terrain with varied height and texture.
- **Camera**: Allows for movement and orientation changes using keyboard and mouse input.
- **Lighting**: Implements a basic lighting model to simulate sunlight on the terrain. (Technically we have a sun but its like far away so we cant see it (It rotates around y of 1000 and go around the map, you can fly up there and see!)

## Project Structure

- **main.cpp**: Entry point of the application, handles initialization and the main render loop.
- **camera.cpp**: Implements the camera class for handling view transformations.
- **terrain.cpp**: Handles the generation and rendering of the terrain.
- **shader.h** and **shader.cpp**: Manage shader compilation and usage.
- **perlin.h** and **perlin.cpp**: Generate Perlin noise for terrain height mapping.
