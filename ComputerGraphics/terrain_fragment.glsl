#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float lightIntensity;

void main() {
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightIntensity * vec3(1.0f, 1.0f, 1.0f);

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightIntensity * vec3(1.0f, 1.0f, 1.0f);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightIntensity * vec3(1.0f, 1.0f, 1.0f);

    // Height-based coloring with smooth transitions
    float height = FragPos.y;

    vec3 baseColor;
    if (height < -10.0) {
        baseColor = vec3(0.59, 0.29, 0.0); // Brown for valleys
    } else if (height < 0.0) {
        float t = (height + 10.0) / 10.0;
        baseColor = mix(vec3(0.59, 0.29, 0.0), vec3(0.0, 0.5, 0.0), t); // Smooth transition from brown to green
    } else if (height < 50.0) {
        float t = (height) / 50.0;
        baseColor = mix(vec3(0.0, 0.5, 0.0), vec3(0.5, 0.8, 0.3), t); // Green to lighter green for hills
    } else if (height < 100.0) {
        float t = (height - 50.0) / 50.0;
        baseColor = mix(vec3(0.5, 0.8, 0.3), vec3(0.7, 0.7, 0.0), t); // Light green to yellowish dry grass
    } else if (height < 150.0) {
        float t = (height - 100.0) / 50.0;
        baseColor = mix(vec3(0.7, 0.7, 0.0), vec3(1.0, 0.9, 0.6), t); // Dry grass to rocky terrain
    } else if (height < 200.0) {
        float t = (height - 150.0) / 50.0;
        baseColor = mix(vec3(1.0, 0.9, 0.6), vec3(1.0, 1.0, 1.0), t); // Rocky terrain to snow-capped peaks
    } else {
        baseColor = vec3(1.0, 1.0, 1.0); // Solid white for high snow peaks
    }

    // Combine results
    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}