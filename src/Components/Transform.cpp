#include "Components/Transform.h"

#include <glm/ext.hpp>

Transform::Transform() :
    m_Position(0.0f, 0.0f, 0.0f),
    m_EulerRotation(0.0f, 0.0f, 0.0f),
    m_Rotation(0.0f),
    m_Forward(0.0f, 0.0f, 1.0f),
    m_Up(0.0f, 1.0f, 0.0f)
{

}

Transform::~Transform()
{

}