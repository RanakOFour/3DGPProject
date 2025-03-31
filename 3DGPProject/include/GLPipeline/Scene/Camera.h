#ifndef CAMERA_H
#define CAMERA_H

#include "GLPipeline/Entity/ShaderProgram.h"
#include "GLPipeline/Entity/Transform.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

class Camera
{
    Transform m_Transform;
    glm::mat4 m_Projection;

    void CalculateView();

    public:
    Camera();
    ~Camera();

    void Use(ShaderProgram* _shader);
    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    glm::vec3 GetForward();
    ShaderProgram* GetShader();
};

#endif