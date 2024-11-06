# Celestial Body Renderer by Andre Marroquin Lab04

This repository contains a software renderer that creates and displays a solar system with various celestial bodies using shaders. The project demonstrates creativity and shader complexity, rendering planets and stars with unique visual effects, solely by manipulating colors without textures or materials.

## Pictures of Planets

## Objective

The goal of this project is to practice creating interesting shaders by varying colors and utilizing parameters at hand. Specifically, the project implements seven celestial bodies, each with its own shader, as follows:
- **Rocky Planet**
- **Gas Giant**
- **Sun (Star)**
- **Earth (with atmosphere)**
- **Mars**
- **Neptune**
- **Star**

## Features

- **Shaders**: Seven distinct shaders are used to render celestial bodies, creating unique effects for each type.
- **Celestial Bodies**:
  - **Sun**: A bright, emissive star.
  - **Rocky Planets**: Earth and Mars, each rendered with varying surface textures.
  - **Gas Giant**: Neptune with gaseous effects.
  - **Star**: Additional star effect for visual interest.
- **User Controls**:
  - Change between celestial bodies using the spacebar.
  - Move the camera view using arrow keys.

## Points and Scoring

The project implements the following features and achieves points based on the provided criteria:

- **Creativity (30 points)**: Unique shaders have been designed for each celestial body, inspired by the solar system but with distinct interpretations.
- **Shader Complexity (30 points)**:
  - The Earth shader includes layers for atmosphere and surface detail.
  - Neptune shader represents gaseous layers with noise.
  - Multiple layers are used in shaders to add depth and detail, achieving the maximum points.
- **Atmospheric Effects (10 points)**: The Earth shader includes moving clouds that simulate atmospheric movement, adding realism.
- **Surface Movement (10 points)**: Earth's surface shader shows changing continents for added complexity.
- **Ring System (20 points)**: Neptune includes a simple ring system implemented through shaders, surrounding the gas giant.
- **Lunar Orbit (20 points)**: A moon orbits the rocky planet, adding an additional layer of visual interest and realism.
- **Additional Celestial Body (10 points)**: An additional unique star type is included in the rendering.

## Project Structure

- `src/main.cpp`: Main program file that initializes SDL, handles user input, and controls rendering.
- `headers/`: Directory containing all header files for classes such as `camera`, `fragment`, `framebuffer`, `model`, and `shaders`.

## Installation and Setup

### Prerequisites

Before starting, ensure the following dependencies are installed:
- **SDL2**: Used for window management and rendering.
- **GLM**: A C++ library for matrix transformations.
- **FastNoiseLite**: For noise generation in shaders.
### Building the Project

Use CMake to build the project:

1. **Create a Build Directory**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ./LAB4GC


### Cloning the Repository
Clone this repository to your local machine:
```bash
git clone https://github.com/andre/Lab4GC.git
cd Lab4GC
