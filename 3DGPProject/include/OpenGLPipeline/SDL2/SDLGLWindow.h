#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "OpenGLPipeline/Scene.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <glm/ext.hpp>

#pragma once
class Entity;
class ShaderProgram;
class SDLGLWindow
{
    SDL_Window* m_Window;
    Scene* m_Scene;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void Update();
    void SetScene(Scene* _scene);
};

#endif