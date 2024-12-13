#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    // Simple shader to render light source as a bright, solid color
    FragColor = vec4(lightColor, 1.0);
}