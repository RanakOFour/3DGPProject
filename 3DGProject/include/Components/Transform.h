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

    void inline Translate(glm::vec3 _translation)
    {
        m_Position += _translation;
    };

    void inline Rotate(glm::vec3 _eulerRotation)
    {
        m_EulerRotation += _eulerRotation;
        m_Forward += _eulerRotation;
        m_Up += _eulerRotation;
        m_Right += _eulerRotation;
    };

    void inline Scale(glm::vec3 _scaleChange) 
    {
        m_Scale += _scaleChange;
    };

    glm::vec3 inline Forward() 
    {
        return m_Forward;
    };

    glm::vec3 inline Up()
    {
        return m_Up;
    };

    glm::vec3 inline Right()
    {
        return m_Right;
    };

    glm::mat4 GetModelMatrix();
};

#endif