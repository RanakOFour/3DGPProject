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

    inline void Translate(glm::vec3 _translation)
    {
        m_Position += _translation;
    };

    inline void Rotate(glm::vec3 _eulerRotation)
    {
        m_EulerRotation += _eulerRotation;

        // Need to find a way to express forward, up n right vectors
        // smth with polar coords
    };

    inline void Scale(glm::vec3 _scaleChange)
    {
        m_Scale += _scaleChange;
    };

    inline glm::vec3* Position()
    {
        return &m_Position;
    };

    inline glm::vec3* Forward()
    {
        return &m_Forward;
    };

    inline glm::vec3* Up()
    {
        return &m_Up;
    };

    inline glm::vec3* Right()
    {
        return &m_Right;
    };

    glm::mat4 GetModelMatrix();
};

#endif