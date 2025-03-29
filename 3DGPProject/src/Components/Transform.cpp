#include "Components/Transform.h"

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