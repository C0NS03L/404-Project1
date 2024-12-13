#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "terrain.h"
#include "sphere.h"  // Assuming a sphere class or model is available

// Window dimensions
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 50.0f, 100.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Lighting
glm::vec3 lightPos(1000, 2000, 1000);
float timeOfDay = 0.0f; // 0.0 to 1.0
float sunSpeed = 0.1f;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
glm::vec3 getSkyboxColor(float timeOfDay);

// Function to update the light position based on time of day
float lightIntensity = 1.0f;

void updateLightPosition() {
    // Calculate sun position based on time of day (rotation around the world)
    float angle = timeOfDay * 360.0f;
    lightPos.x = 100.0f * cos(glm::radians(angle));
    lightPos.z = 100.0f * sin(glm::radians(angle));
    lightIntensity = 0.5f + 0.5f * sin(glm::radians(angle));

}

// Function to update the sun's position
void updateSunPosition() {
    timeOfDay += sunSpeed * deltaTime;
    if (timeOfDay > 1.0f) timeOfDay = 0.0f;
}

int main() {
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fantasy Landscape", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);



    // Build and compile shaders
    Shader terrainShader("terrain_vertex.glsl", "terrain_fragment.glsl");
    Shader lightShader("light_vertex.glsl", "light_fragment.glsl");

    std::cout << "Shader generated successfully\n" << std::endl;

    // Create terrain
    Terrain terrain(200, 200, 10.0f);

    std::cout << "Terrain generated successfully\n" << std::endl;

    // Create a sphere for the sun
    Sphere sun(100, 36, 18); // radius, sectors, stacks

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Update lighting and sun position
        updateSunPosition();
        updateLightPosition();

		//std::cout << "Sun position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
		//std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
		std::cout << "Camera Position: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;

        glm::vec3 skyboxColor = getSkyboxColor(timeOfDay);


        // Render
        glClearColor(skyboxColor.r, skyboxColor.g, skyboxColor.b, 1.0f); // Use skybox color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate terrain shader
        terrainShader.use();

        // View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        terrainShader.setMat4("projection", projection);
        terrainShader.setMat4("view", view);

        // World transformation
        glm::mat4 model = glm::mat4(1.0f);
        terrainShader.setMat4("model", model);

        // Lighting
        terrainShader.setVec3("lightPos", lightPos);
        terrainShader.setVec3("viewPos", camera.Position);
        terrainShader.setFloat("lightIntensity", lightIntensity); // Pass light intensity to shader


        // Render terrain
        terrain.Draw(terrainShader);

        // Render the sun (sphere)
        lightShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos); // Position sun at the light source
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        sun.Draw(lightShader.ID);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Optional: de-allocate all resources
    glfwTerminate();
    return 0;
}

// Process input function
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(SPRINT, deltaTime);
}

// GLFW callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

glm::vec3 getSkyboxColor(float timeOfDay) {
    glm::vec3 morningColor(0.7f, 0.7f, 1.0f);  // Light blue
    glm::vec3 noonColor(0.529f, 0.808f, 0.922f); // Sky blue
    glm::vec3 eveningColor(1.0f, 0.5f, 0.0f);  // Orange
    glm::vec3 nightColor(0.0f, 0.0f, 0.2f);   // Dark blue

    if (timeOfDay < 0.25f) {
        // Morning to noon
        float t = timeOfDay / 0.25f;
        return glm::mix(morningColor, noonColor, t);
    }
    else if (timeOfDay < 0.5f) {
        // Noon to evening
        float t = (timeOfDay - 0.25f) / 0.25f;
        return glm::mix(noonColor, eveningColor, t);
    }
    else if (timeOfDay < 0.75f) {
        // Evening to night
        float t = (timeOfDay - 0.5f) / 0.25f;
        return glm::mix(eveningColor, nightColor, t);
    }
    else {
        // Night to morning
        float t = (timeOfDay - 0.75f) / 0.25f;
        return glm::mix(nightColor, morningColor, t);
    }
}