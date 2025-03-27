#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "Pipeline/Camera.h"
#include "Components/RenderTexture.h"

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
    Camera* m_camera;
    RenderTexture* blue;
    std::vector<std::shared_ptr<Entity>> m_Entities;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void Update();
    void AddEntity(Entity* _object);
};

#endif