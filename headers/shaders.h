#pragma once

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include "FastNoise.h"
#include "uniforms.h"
#include "fragment.h"
#include "noise.h"
#include "print.h"

static int frame = 0;

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    glm::vec3 transformedWorldPosition = glm::vec3(uniforms.model * glm::vec4(vertex.position, 1.0f));

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.tex,
        transformedWorldPosition,
        vertex.position
    };
}

Fragment rockyPlanetShader(Fragment& fragment) {
    // Define a range of earthy tones for a more complex surface appearance
    glm::vec3 baseColor = glm::vec3(0.48f, 0.42f, 0.35f);  // Primary rock color
    glm::vec3 secondaryColor = glm::vec3(0.58f, 0.52f, 0.45f);  // Secondary rock color
    glm::vec3 highlightColor = glm::vec3(0.75f, 0.70f, 0.65f);  // Highlights for edges and ridges
    glm::vec3 shadowColor = glm::vec3(0.30f, 0.28f, 0.26f);  // Shadows in crevices

    // Convert 3D position on sphere to 2D UV coordinates for texturing
    glm::vec3 pos = glm::normalize(fragment.originalPos);
    float u = 0.5f + atan2(pos.z, pos.x) / (2.0f * glm::pi<float>());
    float v = 0.5f - asin(pos.y) / glm::pi<float>();
    glm::vec2 uv = glm::vec2(u, v);
    uv = glm::clamp(uv, 0.0f, 1.0f);

    // Setup noise generators for layered texturing
    FastNoiseLite noiseGenerator1, noiseGenerator2;
    noiseGenerator1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noiseGenerator2.SetNoiseType(FastNoiseLite::NoiseType_Cellular);

    float scale1 = 100.0f;  // Large-scale features
    float scale2 = 400.0f;  // Fine details

    // Generate noise values for texture variation
    float noiseValue1 = noiseGenerator1.GetNoise((uv.x + 0.1f) * scale1, (uv.y + 0.1f) * scale1);
    float noiseValue2 = noiseGenerator2.GetNoise((uv.x + 0.5f) * scale2, (uv.y + 0.5f) * scale2);

    // Normalize and adjust noise values
    noiseValue1 = (noiseValue1 + 1.0f) * 0.5f;
    noiseValue2 = (noiseValue2 + 1.0f) * 0.5f;

    // Combine base and secondary colors based on noise
    glm::vec3 mixedColor = glm::mix(baseColor, secondaryColor, noiseValue1);

    // Apply highlight or shadow based on higher frequency detail noise
    if (noiseValue2 > 0.8) {
        mixedColor = glm::mix(mixedColor, highlightColor, (noiseValue2 - 0.8f) * 5.0f);  // Emphasize highlights
    } else if (noiseValue2 < 0.2) {
        mixedColor = glm::mix(mixedColor, shadowColor, (0.2f - noiseValue2) * 5.0f);  // Emphasize shadows
    }

    // Apply ambient occlusion based on noise to simulate lighting complexity
    mixedColor *= glm::smoothstep(0.2f, 1.0f, noiseValue1);

    // Modulate the final color with the lighting intensity for realism
    glm::vec3 finalColor = mixedColor * fragment.intensity;

    // Convert glm::vec3 color to your Color class, assuming a Color constructor that accepts floats
    fragment.color = Color(finalColor.r, finalColor.g, finalColor.b);

    return fragment;
}

Fragment gasGiantShader(Fragment& fragment) {
    // Base atmospheric colors
    glm::vec3 deepColor = glm::vec3(0.30f, 0.41f, 0.58f);  // Deep blue
    glm::vec3 lightColor = glm::vec3(0.45f, 0.62f, 0.78f);  // Light blue

    // Stormy highlight color
    glm::vec3 stormColor = glm::vec3(0.95f, 0.95f, 0.85f);  // Pale yellow-white

    glm::vec3 pos = glm::normalize(fragment.originalPos);
    float u = 0.5f + atan2(pos.z, pos.x) / (2.0f * glm::pi<float>());
    float v = 0.5f - asin(pos.y) / glm::pi<float>();
    glm::vec2 uv = glm::vec2(u, v);
    uv = glm::clamp(uv, 0.0f, 1.0f);

    // Noise for cloud bands
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float bandNoise = noiseGenerator.GetNoise(uv.x * 300.0f, uv.y * 300.0f);
    bandNoise = glm::smoothstep(0.4f, 0.6f, bandNoise);

    // Mix deep and light atmospheric colors based on noise
    glm::vec3 c = glm::mix(deepColor, lightColor, bandNoise);

    // Add storm highlights
    float stormNoise = noiseGenerator.GetNoise(uv.x * 1000.0f, uv.y * 1000.0f);
    if (stormNoise > 0.8) {
        c = glm::mix(c, stormColor, (stormNoise - 0.8f) * 5.0f);
    }

    // Apply lighting intensity
    c *= fragment.intensity;

    fragment.color = Color(c.r, c.g, c.b);
    return fragment;
}

Fragment sunShader(Fragment& fragment) {
    glm::vec3 coreColor = glm::vec3(1.0, 0.8, 0.0);  // Bright center
    glm::vec3 flareColor = glm::vec3(1.0, 0.5, 0.0);  // Orange flares

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float noiseValue = noise.GetNoise(fragment.originalPos.x * 10, fragment.originalPos.y * 10, fragment.originalPos.z * 10);
    noiseValue = (noiseValue + 1) / 2.0;  // Normalize to [0, 1]

    glm::vec3 color = glm::mix(coreColor, flareColor, noiseValue);
    fragment.color = Color(color.r, color.g, color.b);
    return fragment;
}

Fragment earthShader(Fragment& fragment) {
    glm::vec3 waterColor = glm::vec3(0.0, 0.0, 0.8);
    glm::vec3 landColor = glm::vec3(0.0, 0.5, 0.0);

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float landNoise = noise.GetNoise(fragment.originalPos.x * 20, fragment.originalPos.y * 20);
    landNoise = glm::smoothstep(0.3f, 0.7f, (landNoise + 1.0f) / 2.0f);

    glm::vec3 color = glm::mix(waterColor, landColor, landNoise);
    fragment.color = Color(color.r, color.g, color.b);
    return fragment;
}

Fragment marsShader(Fragment& fragment) {
    glm::vec3 baseColor = glm::vec3(0.7, 0.2, 0.1);  // Reddish soil
    glm::vec3 soilVariation = glm::vec3(0.5, 0.1, 0.1);  // Darker patches

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float soilNoise = noise.GetNoise(fragment.originalPos.x * 50, fragment.originalPos.y * 50);
    soilNoise = glm::smoothstep(0.4f, 0.6f, (soilNoise + 1.0f) / 2.0f);

    glm::vec3 color = glm::mix(baseColor, soilVariation, soilNoise);
    fragment.color = Color(color.r, color.g, color.b);
    return fragment;
}

Fragment neptuneShader(Fragment& fragment) {
    glm::vec3 deepColor = glm::vec3(0.0, 0.0, 0.5);  // Deep blue
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);  // Lighter icy features

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float cloudNoise = noise.GetNoise(fragment.originalPos.x * 40, fragment.originalPos.y * 40);
    cloudNoise = glm::smoothstep(0.45f, 0.55f, (cloudNoise + 1.0f) / 2.0f);

    glm::vec3 color = glm::mix(deepColor, lightColor, cloudNoise);
    fragment.color = Color(color.r, color.g, color.b);
    return fragment;
}

Fragment starShader(Fragment& fragment){
    Color color;

    glm::vec3 secondColor = glm::vec3(55.0f/255.0f, 0.0f/255.0f, 55.0f/255.0f);
    glm::vec3 mainColor = glm::vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);

    glm::vec2 uv = glm::vec2(fragment.originalPos.x * 2.0 - 1.0 , fragment.originalPos.y * 2.0 - 1.0);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    float offsetX = 8000.0f;
    float offsetY = 1000.0f;
    float scale = 30000.0f;

    // Genera el valor de ruido
    float noiseValue = noiseGenerator.GetNoise((uv.x + offsetX) * scale, (uv.y + offsetY) * scale);
    noiseValue = (noiseValue + 1.0f) * 0.9f;

    // Interpola entre el color base y el color secundario basado en el valor de ruido
    secondColor = glm::mix(mainColor, secondColor, noiseValue);

    if (noiseValue > 0.99f){
        // Calcula el valor sinusoide para crear líneas
        float sinValue = glm::sin(uv.y * 20.0f) * 0.1f;

        sinValue = glm::smoothstep(0.8f, 1.0f, sinValue);

        // Combina el color base con las líneas sinusoide
        secondColor = secondColor + glm::vec3 (sinValue);
    }


    // Interpola entre el color base y el color secundario basado en el valor de ruido
    mainColor = glm::mix(mainColor, mainColor, noiseValue);

    // Interpola entre el color base y el color secundario basado en el valor de ruido
    secondColor = glm::mix(mainColor, secondColor, noiseValue);

    color = Color(secondColor.x, secondColor.y, secondColor.z);

    fragment.color = color;

    return fragment;
}





