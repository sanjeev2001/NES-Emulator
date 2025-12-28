#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include "Sprite.h"

class Renderer {
public:
    Renderer(int width, int height, int scale);
    ~Renderer();

    bool Init();
    void Draw(const Sprite& sprite);
    bool ShouldClose();
    void PollEvents();
    void SetTitle(const char* title);

    // Input handling helpers (mapping needed or direct usage)
    bool IsKeyPressed(int key);
    bool IsKeyHeld(int key);

    GLFWwindow* GetWindow() const { return window; }

private:
    int width, height, scale;
    GLFWwindow* window;
    GLuint textureID;

    void UpdateTexture(const Sprite& sprite);
};
