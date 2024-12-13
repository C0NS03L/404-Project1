#include "terrain.h"
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

    // Generate vertices with height variation
    for (int z = 0; z <= depth; ++z) {
        for (int x = 0; x <= width; ++x) {
            // Position
            float posX = x * scale;
            float posZ = z * scale;

            // Height calculation (simple sine wave terrain)
            float height = std::sin(posX * 0.1f) * std::cos(posZ * 0.1f) * 10.0f;

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