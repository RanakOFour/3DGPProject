#include "Physics/Rigidbody.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/SphereShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/MeshShape.h"

#include "glm/ext.hpp"
#include "GL/glew.h"

Rigidbody::Rigidbody(std::weak_ptr<Transform> _transform) :
m_isKinematic(false),
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, -9.8f, 0.0f),
m_LinearDamping(0.3f),
m_Friction(0.1f)
{
	
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::Update(float _delta)
{
    glm::vec3 L_position = m_Transform.lock()->GetPosition();

    glm::vec3 L_posChange = (m_Velocity * _delta) + (0.5f * m_Acceleration * _delta * _delta);

    m_Transform.lock()->Move(L_posChange);

    SetVelocity(m_Velocity + m_Acceleration * _delta);
}

void Rigidbody::AddForce(glm::vec3 _force)
{
	m_Acceleration += _force;
}

void Rigidbody::SetFriction(float _friction)
{
	m_Friction = _friction;
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

bool Rigidbody::GetKinematic()
{
	return m_isKinematic;
}

glm::vec3 Rigidbody::GetVelocity()
{
	return m_Velocity;
}

glm::vec3 Rigidbody::GetAcceleration()
{
	return m_Acceleration;
}

float Rigidbody::GetFriction()
{
    return m_Friction;
}

float Rigidbody::GetLinearDamping()
{
	return m_LinearDamping;
}

std::weak_ptr<Transform> Rigidbody::GetTransform()
{
	return m_Transform;
}