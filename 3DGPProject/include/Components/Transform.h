#ifndef TRANSFORM_H

#define TRANSFORM_H

#include "Components/Component.h"
#include <glm/ext.hpp>
#include <iostream>

class Transform : public Component
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

        // Need to find a way to express forward, up n right vectors
        // smth with polar coords
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