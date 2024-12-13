#include "terrain.h"
#include "perlin.h"
#include <random>
#include <cmath>

Terrain::Terrain(int width, int depth, float scale) {
    generateTerrain(width, depth, scale);
    setupBuffers(); // Add this call to set up OpenGL buffers after terrain generation
}

void Terrain::setupBuffers() {
    // This method sets up the OpenGL buffers for the terrain
    // Note: This is essentially moving the buffer creation code from generateTerrain
    // to a separate method as declared in the header file

    // Create OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::generateTerrain(int width, int depth, float scale) {
    // Clear any existing data
    vertices.clear();
    indices.clear();

    // Noise parameters
    int octaves = 6; // Reduced octaves for less noise detail
    float meshHeight = 15; // Vertical scaling for mountains
    float noiseScale = 1000; // Horizontal scaling
    float persistence = 0.5f;
    float lacunarity = 2.0f;

    std::vector<float> noiseValues;
    std::vector<int> p = get_permutation_vector();
    float maxPossibleHeight = 0;
    float amp = 15.0f;
    float freq = 15.0f;

    // Calculate the max possible height
    for (int i = 0; i < octaves; i++) {
        maxPossibleHeight += amp;
        amp *= persistence;
    }

    // Generate Perlin noise values
    for (int z = 0; z <= depth; ++z) {
        for (int x = 0; x <= width; ++x) {
            amp = 1;
            freq = 1;
            float noiseHeight = 0;
            for (int i = 0; i < octaves; i++) {
                float xSample = (x * scale) / noiseScale * freq;
                float zSample = (z * scale) / noiseScale * freq;
                double perlinValue = perlin_noise(xSample, zSample, 0.5, p); // Pass z as 0.5 for 2D noise
                noiseHeight += perlinValue * amp;
                amp *= persistence;
                freq *= lacunarity;
            }
            noiseValues.push_back(noiseHeight);
        }
    }

    // Normalize the noise values and generate vertices
    for (int z = 0; z <= depth; ++z) {
        for (int x = 0; x <= width; ++x) {
            float posX = x * scale;
            float posZ = z * scale;
            float height = noiseValues[x + z * (width + 1)] * meshHeight;

            // Apply a transformation to create more varied terrain
            height = std::pow(height, 3.0f); // Exaggerate the height difference

            // Vertex data: position (x, y, z), normal, texture coordinates
            vertices.push_back(posX);
            vertices.push_back(height);
            vertices.push_back(posZ);

            // Normal (simplified)
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Texture coordinates
            vertices.push_back(static_cast<float>(x) / width);
            vertices.push_back(static_cast<float>(z) / depth);
        }
    }

    // Generate indices
    for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
            unsigned int current = z * (width + 1) + x;
            unsigned int next = current + width + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    indexCount = indices.size();
}

void Terrain::Draw(Shader& shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}