#ifndef TRANSFORM_H

#define TRANSFORM_H

#include <glm/ext.hpp>
#include <iostream>

class Transform
{
private:

    glm::vec3 m_Position;
    glm::vec3 m_EulerRotation;
    glm::vec3 m_Scale;

    glm::vec3 m_Forward;
    glm::vec3 m_Up;
    glm::vec3 m_Right;

public:
    Transform();
    ~Transform();

    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    void Scale(glm::vec3 _scaleChange);

    glm::vec3& Position();
    glm::vec3 Forward();
    glm::vec3 Up();
    glm::vec3 Right();

    glm::mat4 GetModelMatrix();
};

#endif