#include "Game/Camera.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include "Physics/Transform.h"
#include "GLPipeline/ShaderProgram.h"
#include "GLPipeline/SDLGLWindow.h"
#include "GLPipeline/RenderTexture.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <memory>

Camera::Camera() :
    m_Projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f)),
    m_Transform(),
	m_Target(),
	m_DistanceFromTarget(15.0f),
	m_Pitch(glm::radians(20.0f)),
	m_AngleAroundTarget(0.0f)
{
}

Camera::~Camera()
{

}

void Camera::SetScene(std::shared_ptr<Scene> _scene)
{
    m_Scene = std::weak_ptr<Scene>(_scene);
}

void Camera::SetTarget(std::shared_ptr<GameEntity> _entity)
{
	m_Target = std::weak_ptr<GameEntity>(_entity);
}

void Camera::Update(float _delta, const Uint8* _keys)
{
	// Do rotation with mouse
	glm::vec3 L_mouseDeltas = m_Scene.lock()->GetGame().lock()->GetWindow()->GetMouseInput();

	m_DistanceFromTarget += L_mouseDeltas.z;
	m_Pitch -= glm::radians(L_mouseDeltas.y);
	m_AngleAroundTarget -= glm::radians(L_mouseDeltas.x);

	m_Pitch = glm::clamp(m_Pitch, -glm::radians(89.0f), glm::radians(89.0f));

	glm::vec3 L_playerPos = m_Target.lock()->GetPosition();

	float L_horizontalDist = m_DistanceFromTarget * glm::cos(m_Pitch);
	float L_verticalDist = m_DistanceFromTarget * glm::sin(m_Pitch);
	float L_theta = m_Target.lock()->GetRotation().y + m_AngleAroundTarget;

	glm::vec3 L_cameraPos = glm::vec3(
		L_playerPos.x - (L_horizontalDist * glm::sin(L_theta)),
		L_playerPos.y + L_verticalDist,
		L_playerPos.z - (L_horizontalDist * glm::cos(L_theta))
	);
	
	m_Transform.SetPosition(L_cameraPos);

	printf("Camera:\n	Mouse Deltas: %f, %f, %f\n  Pos: %f, %f, %f\n  Distance: %f\n", L_mouseDeltas.x, L_mouseDeltas.y, L_mouseDeltas.z,
	m_Transform.GetPosition().x, m_Transform.GetPosition().x, m_Transform.GetPosition().z,
	m_DistanceFromTarget);
}

void Camera::Use(ShaderProgram* _shader)
{
    glm::mat4 L_viewMatrix = glm::lookAt(m_Transform.GetPosition(), m_Target.lock()->GetPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
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