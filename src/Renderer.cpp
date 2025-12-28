#include "Renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height, int scale) 
    : width(width), height(height), scale(scale), window(nullptr), textureID(0) {}

Renderer::~Renderer() {
    if (textureID) glDeleteTextures(1, &textureID);
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool Renderer::Init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width * scale, height * scale, "NES Emulator", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    // Setup texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_2D);

    return true;
}

void Renderer::UpdateTexture(const Sprite& sprite) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.width, sprite.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite.pColData.data());
}

void Renderer::Draw(const Sprite& sprite) {
    UpdateTexture(sprite);

    glViewport(0, 0, width * scale, height * scale);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glEnd();

    glfwSwapBuffers(window);
}

bool Renderer::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void Renderer::PollEvents() {
    glfwPollEvents();
}

void Renderer::SetTitle(const char* title) {
    glfwSetWindowTitle(window, title);
}

bool Renderer::IsKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Renderer::IsKeyHeld(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}
