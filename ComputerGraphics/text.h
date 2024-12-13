// Include stb_truetype.h in your project
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// Assuming you have a shader that can render text
Shader textShader("text_vertex.glsl", "text_fragment.glsl");

unsigned int VAO, VBO, textureID;
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

void initTextRendering() {
    // Load font
    std::ifstream fontFile("path/to/font.ttf", std::ios::binary);
    std::vector<unsigned char> fontData((std::istreambuf_iterator<char>(fontFile)),
        std::istreambuf_iterator<char>());

    stbtt_BakeFontBitmap(fontData.data(), 0, 48.0f, bitmap, 512, 512, 32, 96, cdata); // Bake font

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color) {
    textShader.use();
    textShader.setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);

    for (char c : text) {
        stbtt_bakedchar& ch = cdata[c - 32];

        float xpos = x + ch.xoff * scale;
        float ypos = y - (ch.yoff - ch.height) * scale;

        float w = ch.xadvance * scale;
        float h = ch.height * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f }
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.xadvance) * scale; // Advance cursor for next character
    }
}
