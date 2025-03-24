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

    glm::mat4 m_RotationMatrix;
    glm::mat4 m_Projection;
    glm::mat4 m_View;

    void CalculateView();

    public:
    Camera();
    ~Camera();

    void Use();
    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    glm::vec3 GetForward();
    ShaderProgram* GetShader();
};

#endif