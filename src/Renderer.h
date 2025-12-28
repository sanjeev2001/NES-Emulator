#pragma once

#if defined (__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
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
