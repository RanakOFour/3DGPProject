#include "Game/Camera.h"
#include "Physics/Transform.h"
#include "GLPipeline/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Camera::Camera() :
    m_Projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f)),
    m_Transform()
{
}

Camera::~Camera()
{

}

void Camera::Use(ShaderProgram* _shader)
{
    glm::mat4 L_viewMatrix = glm::lookAt(m_Transform.GetPosition(), m_Transform.GetPosition() + front, -up);
	_shader->SetUniform("u_View", L_viewMatrix);
    
	_shader->SetUniform("u_Projection", m_Projection);
}

void Camera::Translate(glm::vec3 _translate)
{
    m_Transform.Move(_translate);
}

void Camera::Rotate(glm::vec3 _eulerRotation)
{
    m_Transform.SetRotation(m_Transform.EulerAngles() + _eulerRotation);
}