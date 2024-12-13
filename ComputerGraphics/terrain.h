#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

class Terrain {
public:
    Terrain(int width, int depth, float scale);
    void Draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;
    int indexCount;

    // Store vertex and index data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void setupBuffers();
    void generateTerrain(int width, int depth, float scale);
};
#endif