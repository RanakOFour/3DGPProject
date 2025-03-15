#include "Pipeline/ShaderProgram.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <glm/ext.hpp>

#pragma once
class Object;
class SDLGLWindow
{
    SDL_Window* m_Window;
    std::vector<std::shared_ptr<Object>> m_Objects;
    ShaderProgram m_Shader;
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_model;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void Update();
    void SetShaderProgram(ShaderProgram* _program);
    void AddObject(Object* _object);
};