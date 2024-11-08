# Celestial Body Renderer by Andre Marroquin Lab04

This repository contains a software renderer that creates and displays a solar system with various celestial bodies using shaders. The project demonstrates creativity and shader complexity, rendering planets and stars with unique visual effects, solely by manipulating colors without textures or materials.

## Pictures of Planets
### Earth
![Earth](./earth.png)

### Gas Giant
![Gas Giant](./gas.png)

### Mars
![Mars](./mars.png)

### Neptune
![Neptune](./neptune.png)

### Rocky Planet
![Rocky Planet](./rocky.png)

### Star
![Star](./star.png)

### Sun
![Sun](./sun.png)
## Objective

The goal of this project is to practice creating interesting shaders by varying colors and utilizing parameters at hand. Specifically, the project implements seven celestial bodies, each with its own shader, as follows:
- **Rocky Planet**
- **Gas Giant**
- **Sun**
- **Earth**
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

### Creativity and Design (30 Points)
- **Creativity in Shaders**: Each celestial body in our solar system model features uniquely designed shaders, drawing inspiration from real astronomical phenomena while introducing creative, stylized interpretations that enhance the visual experience.

### Shader Complexity (30 Points)
- **Complex Layered Shaders**:
  - **Earth Shader**: Incorporates multiple layers to represent the atmosphere and intricate surface details, simulating realistic environmental effects.
  - **Neptune Shader**: Visualizes the complex gaseous atmosphere using noise-based layers that add depth and movement to the planet.
  - **Depth and Detail**: Utilization of multiple shader layers across different celestial bodies ensures a rich, detailed rendering, fully meeting the criteria for maximum points.

### Dynamic Effects (20 Points)
- **Atmospheric Effects (10 Points)**: Dynamic cloud movements on Earth are rendered using shaders that simulate atmospheric dynamics, adding a layer of realism to the planet.
- **Surface Movement (10 Points)**: The shader for Earth also includes animations that depict the shifting of continents, providing a dynamic visual representation of geological changes over time.

### Special Features (30 Points)
- **Lunar Orbit (20 Points)**: A shader-driven moon orbits a rocky planet, contributing to the dynamic complexity and realism of the scene.
- **Additional Celestial Body (10 Points)**: The simulation includes a unique star type, adding diversity to the celestial entities rendered in the project.

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
