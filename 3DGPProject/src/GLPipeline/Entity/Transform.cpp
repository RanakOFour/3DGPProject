#include "GLPipeline/Entity/Transform.h"

#include <glm/ext.hpp>

Transform::Transform() :
    m_Position(0.0f, 0.0f, 0.0f),
    m_EulerRotation(0.0f, 0.0f, 0.0f),
    m_Scale(1.0f, 1.0f, 1.0f),
    m_Forward(0.0f, 0.0f, 1.0f),
    m_Up(0.0f, 1.0f, 0.0f),
    m_Right(1.0f, 0.0f, 0.0f)
{

}

Transform::~Transform()
{

}

void Transform::Translate(glm::vec3 _translation)
{
    m_Position += _translation;
};

void Transform::Rotate(glm::vec3 _eulerRotation)
{
    m_EulerRotation += _eulerRotation;

    // Need to find a way to express forward, up n right vectors
    // smth with polar coords
};

void Transform::Scale(glm::vec3 _scaleChange)
{
    m_Scale += _scaleChange;
};

glm::vec3 Transform::Position()
{
    return m_Position;
};

glm::vec3* Transform::Forward()
{
    return &m_Forward;
};

glm::vec3* Transform::Up()
{
    return &m_Up;
};

glm::vec3* Transform::Right()
{
    return &m_Right;
};

glm::mat4 Transform::GetModelMatrix()
{
    glm::mat4 L_modelMatrix = glm::mat4(1.0f);

    
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    L_modelMatrix = glm::scale(L_modelMatrix, m_Scale);
    
    L_modelMatrix = glm::translate(L_modelMatrix, m_Position);

    return L_modelMatrix;
}