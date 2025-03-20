#ifndef CAMERA_H
#define CAMERA_H

#include "Pipeline/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

class Camera
{
    ShaderProgram m_Shader;

    glm::vec3 m_Position;
    glm::vec3 m_EulerRotation;

    glm::vec3 m_Forward;
    glm::vec3 m_Up;
    glm::mat4 m_Projection;
    glm::mat4 m_View;

    void CalculateUp();

    public:
    Camera();
    ~Camera();

    void Use();
    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    ShaderProgram* GetShader();
};

#endif