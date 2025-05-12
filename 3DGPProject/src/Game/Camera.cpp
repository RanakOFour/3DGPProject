#include "Game/Camera.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include "Physics/Transform.h"
#include "GLPipeline/ShaderProgram.h"
#include "GLPipeline/SDLGLWindow.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <memory>

Camera::Camera() :
    m_Projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f)),
    m_Transform(),
	m_Target()
{
}

Camera::~Camera()
{

}

void Camera::SetScene(std::shared_ptr<Scene> _scene)
{
    m_Scene = std::weak_ptr<Scene>(_scene);
	m_Target = m_Scene.lock()->GetPlayer()->GetPosition();
}

void Camera::Update(float _delta, const Uint8* _keys)
{
    if (_keys[SDL_SCANCODE_Q])
	{
		Translate(m_Transform.Up() * _delta * 3.0f);
	}

	if (_keys[SDL_SCANCODE_E])
	{
		Translate(-m_Transform.Up() * _delta * 3.0f);
	}

    if (_keys[SDL_SCANCODE_D])
	{
		Translate(m_Transform.Right() * _delta * 3.0f);
	}

	if (_keys[SDL_SCANCODE_A])
	{
		Translate(-m_Transform.Right() * _delta * 3.0f);
	}
		
	if (_keys[SDL_SCANCODE_W])
	{
		Translate(m_Transform.Forward() * _delta * 3.0f);
	}

	if (_keys[SDL_SCANCODE_S])
	{
		Translate(-m_Transform.Forward() * _delta * 3.0f);
	}

    // Do rotation with mouse
	glm::vec3 L_mouseDeltas = m_Scene.lock()->GetGame().lock()->GetWindow()->GetMouseInput();

    glm::vec3 L_eulers = m_Transform.EulerAngles();

	L_eulers.y += glm::radians(L_mouseDeltas.x);
	L_eulers.x -= glm::radians(L_mouseDeltas.y);
	
	L_eulers.x = glm::clamp(L_eulers.x, -glm::radians(89.0f), glm::radians(89.0f));

    m_Transform.SetRotation(L_eulers);


	printf("Camera:\n	Mouse Deltas: %f, %f\n  Pos: %f, %f, %f\n  Rot: %f, %f, %f\n", L_mouseDeltas.x, L_mouseDeltas.y,
	m_Transform.GetPosition().x, m_Transform.GetPosition().x, m_Transform.GetPosition().z,
	m_Transform.EulerAngles().x, m_Transform.EulerAngles().y, m_Transform.EulerAngles().z);
}

void Camera::Use(ShaderProgram* _shader)
{
    glm::mat4 L_viewMatrix = glm::lookAt(m_Transform.GetPosition(), m_Target, -m_Transform.Up());
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