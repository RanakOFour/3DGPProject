#include "OpenGLPipeline/Camera.h"
#include "OpenGLPipeline/ShaderProgram.h"

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

void Camera::CalculateView()
{

}

void Camera::Use(ShaderProgram* _shader)
{
    //CalculateView();

	_shader->SetUniform("u_View", m_Transform.GetModelMatrix());
	_shader->SetUniform("u_Projection", m_Projection);
}

void Camera::Translate(glm::vec3 _translate)
{
    m_Transform.Translate(_translate);
}

void Camera::Rotate(glm::vec3 _eulerRotation)
{
    m_Transform.Rotate(_eulerRotation);
}