#include "Pipeline/Camera.h"
#include "Pipeline/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Camera::Camera() :
    m_Projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f)),
    m_Transform(),
    m_Shader("./resources/shaders/default/vert.vs", "./resources/shaders/default/frag.fs")
{
    
}

Camera::~Camera()
{

}

void Camera::CalculateView()
{

}

void Camera::Use()
{
    m_Shader.Use();

    //CalculateView();

	m_Shader.SetUniform("u_View", m_Transform.GetModelMatrix());
	m_Shader.SetUniform("u_Projection", m_Projection);
}

void Camera::Translate(glm::vec3 _translate)
{
    m_Transform.m_Position += _translate;
    
    printf("New camera position: %s\n", glm::to_string(m_Transform.m_Position).c_str());
}

void Camera::Rotate(glm::vec3 _eulerRotation)
{
    m_Transform.m_EulerRotation += _eulerRotation;

    //m_View = glm::rotate(m_View, _eulerRotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    
    printf("New camera rotation: %s\n", glm::to_string(m_Transform.m_EulerRotation).c_str());

}

ShaderProgram* Camera::GetShader()
{
    return &m_Shader;
}