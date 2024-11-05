#pragma once  

#include <array>       
#include <algorithm>   
#include <glm/glm.hpp>
#include <limits>      
#include <mutex>       
#include "color.h"    
#include "fragment.h"  

// Constants for screen dimensions
constexpr size_t SCREEN_WIDTH = 800;
constexpr size_t SCREEN_HEIGHT = 600;

// Predefined blank color with maximum depth to represent an empty pixel
FragColor blank{
  Color{0, 0, 0},
  std::numeric_limits<double>::max()
};

// Framebuffer for storing the color and depth information of each pixel
std::array<FragColor, SCREEN_WIDTH * SCREEN_HEIGHT> framebuffer;

// Array of mutexes for managing concurrency at the pixel level
std::array<std::mutex, SCREEN_WIDTH * SCREEN_HEIGHT> mutexes;

// Function to update a point in the framebuffer if the new fragment is closer
void point(Fragment f) {
    std::lock_guard<std::mutex> lock(mutexes[f.y * SCREEN_WIDTH + f.x]); // Lock the mutex for the specific pixel
    if (f.z < framebuffer[f.y * SCREEN_WIDTH + f.x].z) {  // Check if the fragment is closer than the current one
       framebuffer[f.y * SCREEN_WIDTH + f.x] = FragColor{f.color, f.z}; // Update framebuffer with new fragment
    }
}

// Function to clear the framebuffer with the blank color
void clearFramebuffer() {
    std::fill(framebuffer.begin(), framebuffer.end(), blank); // Reset all pixels to blank
}

// Function to render the framebuffer contents to an SDL texture and present it
void renderBuffer(SDL_Renderer* renderer) {
    // Create and configure SDL texture for pixel manipulation
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    void* texturePixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texturePixels, &pitch); // Lock texture for pixel data update
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Set blending mode

    Uint32* texturePixels32 = static_cast<Uint32*>(texturePixels); // Prepare pixel array
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int framebufferY = SCREEN_HEIGHT - y - 1;  // Convert framebuffer coordinate to SDL texture coordinate
            int index = y * (pitch / sizeof(Uint32)) + x;
            const Color& color = framebuffer[framebufferY * SCREEN_WIDTH + x].color;
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a); // Map framebuffer colors to texture
        }
    }

    SDL_UnlockTexture(texture); // Unlock texture after update
    SDL_Rect textureRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, texture, NULL, &textureRect); // Copy texture to renderer
    SDL_DestroyTexture(texture); // Cleanup texture

    SDL_RenderPresent(renderer); // Present the renderer contents to the display
}
