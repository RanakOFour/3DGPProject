#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "Pipeline/Camera.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <glm/ext.hpp>

#pragma once
class Object;
class ShaderProgram;
class SDLGLWindow
{
    SDL_Window* m_Window;
    Camera* m_camera;
    std::vector<std::shared_ptr<Object>> m_Objects;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void Update();
    void SetShaderProgram(ShaderProgram* _program);
    void AddObject(Object* _object);
};

#endif