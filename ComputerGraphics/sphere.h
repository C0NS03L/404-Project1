#ifndef SPHERE_H
#define SPHERE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

class Sphere {
public:
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
    void Draw(unsigned int shaderID);

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    void initSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
};

#endif
#pragma once
