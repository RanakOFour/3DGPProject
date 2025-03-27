#ifndef TRANSFORM_H

#define TRANSFORM_H

#include <glm/ext.hpp>

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

    void inline Translate(glm::vec3 _translation);
    void inline Rotate(glm::vec3 _eulerRotation);
    void inline Scale(glm::vec3 _scaleChange);

    glm::vec3 inline Forward();
    glm::vec3 inline Up();
    glm::vec3 inline Right();
    glm::mat4 GetModelMatrix();
};

#endif