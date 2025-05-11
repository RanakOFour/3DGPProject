#include "Physics/Transform.h"

#include <glm/ext.hpp>

Transform::Transform() :
    m_Position(0.0f, 0.0f, 0.0f),
    m_Rotation(),
    m_Scale(1.0f),
    m_Up(0.0f, 1.0f, 0.0f),
    m_Forward(0.0, 0.0, 1.0f),
    m_Right(1.0f, 0.0f, 0.0f)
{

}

Transform::~Transform()
{

}

void Transform::Move(glm::vec3 _translation)
{
    SetPosition(m_Position + _translation);
};

glm::vec3& Transform::GetPosition()
{
    return m_Position;
};

void Transform::SetPosition(glm::vec3 _position)
{
    m_Position = _position;
}

glm::quat Transform::GetRotation()
{
    return m_Rotation;
}

void Transform::SetRotation(glm::quat _rot)
{
    m_Rotation = _rot;
    m_Rotation = glm::normalize(m_Rotation);

    glm::mat3 L_rotMat = glm::mat3_cast(m_Rotation);

    m_Right = L_rotMat[0];
    m_Up = L_rotMat[1];
    m_Forward = L_rotMat[2];
}

glm::vec3 Transform::EulerAngles()
{
    return glm::eulerAngles(m_Rotation);
}

void Transform::SetRotation(glm::vec3 _eulerAngles)
{
    m_Rotation = glm::quat(_eulerAngles);
    m_Rotation = glm::normalize(m_Rotation);

    glm::mat3 L_rotMat = glm::mat3_cast(m_Rotation);

    m_Right = L_rotMat[0];
    m_Up = L_rotMat[1];
    m_Forward = L_rotMat[2];
}

glm::vec3 Transform::GetScale()
{
    return m_Scale;
}

void Transform::SetScale(glm::vec3 _scale)
{
    m_Scale = _scale;
}