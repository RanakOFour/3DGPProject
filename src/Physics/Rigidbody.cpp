#include "Physics/Rigidbody.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/MeshShape.h"

#include "glm/ext.hpp"
#include "GL/glew.h"

Rigidbody::Rigidbody(std::weak_ptr<Transform> _transform, float _mass) :
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, -9.8f, 0.0f),
m_Mass(_mass)
{
	
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::Update(float _delta)
{
	if(m_Mass == INFINITY) { return; }
	
	m_Force += m_Mass * m_Acceleration;

    float L_inverseMass = 1.0f / m_Mass;

	m_Velocity += (m_Force * L_inverseMass) * _delta;

	glm::vec3 L_posChange = m_Velocity * _delta;

	m_Transform.lock()->Move(L_posChange);

	// Damping
	m_Velocity -= m_Velocity * 0.4f * _delta;

	m_Force.x = 0.0f;
	m_Force.y = 0.0f;
	m_Force.z = 0.0f;
}

void Rigidbody::AddForce(glm::vec3 _force)
{
	m_Force += _force;
}

void Rigidbody::SetVelocity(glm::vec3 _velocity)
{
	m_Velocity = _velocity;
}

void Rigidbody::SetAcceleration(glm::vec3 _accel)
{
	m_Acceleration = _accel;
}

void Rigidbody::SetTransform(std::weak_ptr<Transform> _transform)
{
	m_Transform = _transform;
}

glm::vec3 Rigidbody::GetVelocity()
{
	return m_Velocity;
}

glm::vec3 Rigidbody::GetAcceleration()
{
	return m_Acceleration;
}

std::weak_ptr<Transform> Rigidbody::GetTransform()
{
	return m_Transform;
}