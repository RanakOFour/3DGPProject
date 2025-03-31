#ifndef CAMERA_H
#define CAMERA_H

#include "OpenGLPipeline/ShaderProgram.h"
#include "OpenGLPipeline/Transform.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

class Camera
{
    ShaderProgram m_Shader;

    Transform m_Transform;
    
    glm::mat4 m_Projection;

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