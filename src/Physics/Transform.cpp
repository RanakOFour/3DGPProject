#include "Physics/Transform.h"

#include <glm/ext.hpp>

Transform::Transform() :
    m_Position(0.0f, 0.0f, 0.0f),
    m_Rotation(0.0f, 0.0f, 0.0f, 0.0f),
    m_Scale(1.0f, 1.0f, 1.0f),
    m_Up(0.0f, 1.0f, 0.0f),
    m_Forward(0.0, 0.0, 1.0f),
    m_Right(1.0f, 0.0f, 0.0f)
{

}

Transform::~Transform()
{

}

void Transform::SetPosition(glm::vec3 _position)
{
    m_Position = _position;
}

glm::vec3 Transform::GetPosition()
{
    return m_Position;
}

void Transform::Move(glm::vec3 _translation)
{
    SetPosition(m_Position + _translation);
}

glm::quat Transform::GetRotation()
{
    return m_Rotation;
}

void Transform::SetRotation(glm::quat _rot)
{
    m_Rotation = _rot;
    m_Rotation = glm::normalize(m_Rotation);

    glm::vec3 L_eulers = EulerAngles();

    glm::vec3 L_front = glm::vec3(
		glm::cos(L_eulers.y) * glm::cos(L_eulers.x),
		glm::sin(L_eulers.x),
		glm::sin(L_eulers.y) * glm::cos(L_eulers.x)
	);

	m_Forward = glm::normalize(L_front);
	m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_Up = glm::normalize(glm::cross(m_Forward, m_Right));
}

glm::vec3 Transform::EulerAngles()
{
    return glm::eulerAngles(m_Rotation);
}

void Transform::SetRotation(glm::vec3 _eulerAngles)
{
    m_Rotation = glm::quat(_eulerAngles);
    m_Rotation = glm::normalize(m_Rotation);

	glm::vec3 L_eulers = EulerAngles();

    glm::vec3 L_front = glm::vec3(
		glm::cos(L_eulers.y) * glm::cos(L_eulers.x),
		glm::sin(L_eulers.x),
		glm::sin(L_eulers.y) * glm::cos(L_eulers.x)
	);

	m_Forward = glm::normalize(L_front);
	m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_Up = glm::normalize(glm::cross(m_Forward, m_Right));
}

glm::vec3 Transform::GetScale()
{
    return m_Scale;
}

void Transform::SetScale(glm::vec3 _scale)
{
    m_Scale = _scale;
}

glm::mat4 Transform::ModelMatrix()
{
    glm::mat4 L_modelMatrix = glm::mat4(1.0f);
    glm::vec3 m_EulerRotation = glm::eulerAngles(m_Rotation);
    
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    L_modelMatrix = glm::rotate(L_modelMatrix, glm::radians(m_EulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    L_modelMatrix = glm::scale(L_modelMatrix, m_Scale);
    
    L_modelMatrix = glm::translate(L_modelMatrix, m_Position);

    return L_modelMatrix;
}