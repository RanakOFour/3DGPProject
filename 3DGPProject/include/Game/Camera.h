#ifndef CAMERA_H
#define CAMERA_H

#include "GLPipeline/ShaderProgram.h"
#include "Physics/Transform.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

class Camera
{
    Transform m_Transform;
    glm::mat4 m_Projection;

    public:
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    Camera();
    ~Camera();

    void Use(ShaderProgram* _shader);
    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    void SetPosition(glm::vec3 _pos);
    void SetRotation(glm::vec3 _euler);
    
    glm::vec3 GetPosition() { return m_Transform.GetPosition(); };
};

#endif