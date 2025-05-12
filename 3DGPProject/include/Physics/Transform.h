#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/ext.hpp>
#include <iostream>

class Transform
{
private:

    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;

    glm::vec3 m_Up;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;

public:
    Transform();
    ~Transform();

    void Move(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);

    glm::vec3& GetPosition();
    void SetPosition(glm::vec3 _position);
    
    glm::quat GetRotation();
    void SetRotation(glm::quat _rotation);

    glm::vec3 EulerAngles();
    void SetRotation(glm::vec3 _eulerAngles);

    glm::vec3 GetScale();
    void SetScale(glm::vec3 _scale);

    glm::mat4 ModelMatrix();

    glm::vec3 Up() {return m_Up;};
    glm::vec3 Forward() {return m_Forward;};
    glm::vec3 Right() {return m_Right;};
};

#endif