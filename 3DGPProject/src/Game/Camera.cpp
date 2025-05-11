#include "Game/Camera.h"
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
    glm::mat4 L_modelMatrix = glm::translate(glm::mat4(1.0f), m_Transform.GetPosition());

	L_modelMatrix *= glm::mat4_cast(m_Transform.GetRotation());

	L_modelMatrix = glm::scale(L_modelMatrix, m_Transform.GetScale());

	_shader->SetUniform("u_View", L_modelMatrix);
	_shader->SetUniform("u_Projection", m_Projection);
}

void Camera::Translate(glm::vec3 _translate)
{
    m_Transform.Move(_translate);
}

void Camera::Rotate(glm::vec3 _eulerRotation)
{
    m_Transform.SetRotation(_eulerRotation);
}