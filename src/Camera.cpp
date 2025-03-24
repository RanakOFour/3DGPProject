#include "Pipeline/Camera.h"
#include "Pipeline/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Camera::Camera() :
    m_Position(0.0f, 0.0f, 3.0f),
    m_EulerRotation(0.0f, 0.0f, 0.0f),
    m_Projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f)),
    m_RotationMatrix(),
    m_View(1.0f),
    m_Shader("./resources/shaders/default/vert.vs", "./resources/shaders/default/frag.fs")
{
    
}

Camera::~Camera()
{

}

void Camera::CalculateView()
{
    glm::vec3 normalPos = glm::normalize(m_Position); 
    

    m_View = glm::lookAt(m_Position,
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Use()
{
    m_Shader.Use();

    CalculateView();

	m_Shader.SetUniform("u_View", m_View);
	m_Shader.SetUniform("u_Projection", m_Projection);
}

void Camera::Translate(glm::vec3 _translate)
{
    m_Position += _translate;
    printf("New camera position: %s\n", glm::to_string(m_Position).c_str());
}

void Camera::Rotate(glm::vec3 _eulerRotation)
{
    m_EulerRotation += _eulerRotation;
}

ShaderProgram* Camera::GetShader()
{
    return &m_Shader;
}