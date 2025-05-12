#include "Physics/Physics.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/SphereShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/MeshShape.h"

#include "glm/ext.hpp"
#include "GL/glew.h"

Physics::Physics(std::weak_ptr<Transform> _transform) :
m_isKinematic(false),
m_Shape(),
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, -9.8f, 0.0f),
m_PreviousPos(0.0f, 0.0f, 0.0f),
m_Force(0.0f, 0.0f, 0.0f),
m_Torque(0.0f, 0.0f, 0.0f),
m_AngularVelocity(0.0f, 0.0f, 0.0f),
m_InverseBodyInertiaTensor(0.0f),
m_InverseInertiaTensor(0.0f),
m_Restitution(0.2f),
m_LinearDamping(0.3f),
m_AngularDamping(0.2f),
m_Friction(0.5f)
{
	
}

Physics::Physics(float _radius, std::weak_ptr<Transform> _transform) :
m_isKinematic(false),
m_Shape(),
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, -9.8f, 0.0f),
m_PreviousPos(0.0f, 0.0f, 0.0f),
m_Force(0.0f, 0.0f, 0.0f),
m_Torque(0.0f, 0.0f, 0.0f),
m_AngularVelocity(0.0f, 0.0f, 0.0f),
m_InverseBodyInertiaTensor(0.0f),
m_InverseInertiaTensor(0.0f),
m_Restitution(0.2f),
m_LinearDamping(0.3f),
m_AngularDamping(0.2f),
m_Friction(0.5f)
{
    CollisionShape* L_sphere = (CollisionShape*)(new SphereShape(_radius));
    m_Shape = std::unique_ptr<CollisionShape>((CollisionShape*)L_sphere);
	SetMass(1.0f);
}

Physics::Physics(glm::vec3 _size, std::weak_ptr<Transform> _transform) :
m_isKinematic(false),
m_Shape(),
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, -9.8f, 0.0f),
m_PreviousPos(0.0f, 0.0f, 0.0f),
m_Force(0.0f, 0.0f, 0.0f),
m_Torque(0.0f, 0.0f, 0.0f),
m_AngularVelocity(0.0f, 0.0f, 0.0f),
m_InverseBodyInertiaTensor(0.0f),
m_InverseInertiaTensor(0.0f),
m_Restitution(0.2f),
m_LinearDamping(0.3f),
m_AngularDamping(0.2f),
m_Friction(0.5f)
{
    CollisionShape* L_cube = (CollisionShape*)(new CubeShape(_size * 0.5f));
    m_Shape = std::unique_ptr<CollisionShape>(L_cube);
	SetMass(1.0f);
}

Physics::Physics(std::shared_ptr<Model> _model, std::shared_ptr<Transform> _transform) :
m_isKinematic(false),
m_Shape(),
m_Transform(_transform),
m_Velocity(0.0f, 0.0f, 0.0f),
m_Acceleration(0.0f, 0.0f, 0.0f),
m_PreviousPos(0.0f, 0.0f, 0.0f),
m_Force(0.0f, 0.0f, 0.0f),
m_Torque(0.0f, 0.0f, 0.0f),
m_AngularVelocity(0.0f, 0.0f, 0.0f),
m_InverseBodyInertiaTensor(0.0f),
m_InverseInertiaTensor(0.0f),
m_Restitution(0.2f),
m_LinearDamping(0.3f),
m_AngularDamping(0.2f),
m_Friction(0.5f)
{
	CollisionShape* L_mesh = (CollisionShape*)(new MeshShape(_model));
    m_Shape = std::unique_ptr<CollisionShape>(L_mesh);
	SetMass(1.0f);
}

Physics::~Physics()
{

}

void Physics::Update(float _delta)
{
    if(m_isKinematic)
	{
		Kinematic(_delta);
		return;
	}

    //printf("Current Position: %f, %f, %f\n", m_Transform.lock()->GetPosition().x, m_Transform.lock()->GetPosition().y, m_Transform.lock()->GetPosition().z);

	// Ignore objects that should not move
	if(m_Mass == INFINITY){ return; }

	m_Force += m_Acceleration * m_Mass;

	Euler(_delta);

	SetVelocity(m_Velocity * glm::max(1.0f - m_LinearDamping * _delta, 0.0f));
	
	// Integrate angular motion
	glm::quat L_rotation = m_Transform.lock()->GetRotation();
	glm::mat3 L_rotationMatrix = glm::mat3(L_rotation);
	m_InverseInertiaTensor = L_rotationMatrix * m_InverseBodyInertiaTensor * glm::transpose(L_rotationMatrix);
	
	glm::vec3 L_angularAccel = m_InverseInertiaTensor * m_Torque;
	m_AngularVelocity += L_angularAccel * _delta;
	
	glm::vec3 L_angularChange = m_AngularVelocity * 0.5f * _delta;
	L_rotation += (glm::quat(L_angularChange.x, L_angularChange.y, L_angularChange.z, 0.0f) * L_rotation);
	L_rotation = glm::normalize(L_rotation);

	m_Transform.lock()->SetRotation(L_rotation);

	m_AngularVelocity *= glm::max(1.0f - m_AngularDamping * _delta, 0.0f);

	m_Force.x = 0.0f;
	m_Force.y = 0.0f;
	m_Force.z = 0.0f;

	m_Torque.x = 0.0f;
	m_Torque.y = 0.0f;
	m_Torque.z = 0.0f;
}

void Physics::AddForce(glm::vec3 _force)
{
	m_Force += _force;
}

void Physics::AddForceAtPosition(glm::vec3 _force, glm::vec3 _position)
{
	m_Force += _force;
	m_Torque += glm::cross(_position, _force);
}

void Physics::AddTorque(glm::vec3 _torque)
{
	//printf("Torque Added: %f, %f, %f\n", _torque.x, _torque.y, _torque.z);
	m_Torque += _torque;
}

glm::vec3 Physics::ComputeTorque(glm::vec3 _torqueArm, glm::vec3 _contactForce)
{
	return glm::cross(_torqueArm, _contactForce);
}

void Physics::Kinematic(float _delta)
{
	glm::vec3 L_position = m_Transform.lock()->GetPosition();

    glm::vec3 L_posChange = (m_Velocity * _delta) + (0.5f * m_Acceleration * _delta * _delta);

    m_Transform.lock()->Move(L_posChange);

    SetVelocity(m_Velocity + m_Acceleration * _delta);

    if(L_position.y <= 0.0f)
    {
        glm::vec3 L_correctedPos = glm::vec3(0.0f, -L_position.y, 0.0f);
        m_Transform.lock()->Move(L_correctedPos);
    }
}

void Physics::Euler(float _delta)
{
	//printf("Euler Integration:\n	Mass: %f\n	Velocity: %f, %f, %f\n	Position: %f, %f, %f\n	Delta: %f\n",
    //    m_Mass, m_Velocity.x, m_Velocity.y, m_Velocity.z, m_Transform.lock()->GetPosition().x, m_Transform.lock()->GetPosition().y, m_Transform.lock()->GetPosition().z, _delta);
	
	
	float L_inverseMass = 1.0f / m_Mass;

	SetVelocity(m_Velocity + (m_Force * L_inverseMass) * _delta);

	m_Transform.lock()->Move(m_Velocity * _delta);
}

void Physics::SetFriction(float _friction)
{
	m_Friction = _friction;
}


void Physics::SetRestitution(float _restitution)
{
	m_Restitution = _restitution;
}

void Physics::SetLinearDamping(float _damp)
{
	m_LinearDamping = _damp;
}

void Physics::SetAngularDamping(float _damp)
{
	m_AngularDamping = _damp;
}

void Physics::SetMass(float _mass)
{
	m_Mass = _mass;

	if(m_Mass == INFINITY)
	{ 
		m_InverseBodyInertiaTensor = glm::mat3(0.0f);
		 return;
	}

	// Recalculate the inverse tensors
	glm::mat3 L_bodyInertiaTensor;

	if(m_Shape->GetType() == ShapeType::Sphere)
	{
		float L_radius = ((SphereShape*)m_Shape.get())->GetRadius();
		float L_bodyInertia = (2.0f / 5.0f) * m_Mass * L_radius * L_radius;
		L_bodyInertiaTensor = glm::mat3(
			L_bodyInertia, 0.0f, 0.0f,
			0.0f, L_bodyInertia, 0.0f,
			0.0f, 0.0f, L_bodyInertia
			);
	}
	else
	{
		glm::vec3 L_size = ((CubeShape*)m_Shape.get())->GetHalfSize() * 2.0f;
		float L_val1 = (1.0f / 12.0f) * m_Mass * (L_size.z * L_size.z + L_size.y * L_size.y);
		float L_val2 = (1.0f / 12.0f) * m_Mass * (L_size.x * L_size.x + L_size.z * L_size.z);
		float L_val3 = (1.0f / 12.0f) * m_Mass * (L_size.x * L_size.x + L_size.y * L_size.y);

		L_bodyInertiaTensor = glm::mat3(
			L_val1, 0.0f, 0.0f,
			0.0f, L_val2, 0.0f,
			0.0f, 0.0f, L_val3
		);
	}

	m_InverseBodyInertiaTensor = glm::inverse(L_bodyInertiaTensor);

    glm::mat3 L_rotationMatrix = glm::mat3(m_Transform.lock()->GetRotation());

	m_InverseInertiaTensor = L_rotationMatrix * m_InverseBodyInertiaTensor * glm::transpose(L_rotationMatrix);
}

void Physics::SetVelocity(glm::vec3 _velocity)
{
	if(glm::length(_velocity) < 0.01f)
	{
		_velocity = glm::vec3(0.0f);
	}

	//m_Velocity = glm::round(_velocity * 1e4f) * 1e-4f; 
	m_Velocity = _velocity;
}

void Physics::SetAngularVelocity(glm::vec3 _angVel)
{
	if(glm::length(_angVel) < 0.01f)
	{
		_angVel = glm::vec3(0.0f);
	}

	m_AngularVelocity = glm::round(_angVel * 1e4f) * 1e-4f; 
}

void Physics::SetAcceleration(glm::vec3 _accel)
{
	m_Acceleration = _accel;
}

void Physics::SetCollisionShape(CollisionShape* _shape)
{
	m_Shape = std::unique_ptr<CollisionShape>(_shape);
}

void Physics::SetTransform(std::weak_ptr<Transform> _transform)
{
	m_Transform = _transform;
}

bool Physics::GetKinematic()
{
	return m_isKinematic;
}

glm::vec3 Physics::GetVelocity()
{
	return m_Velocity;
}

glm::vec3 Physics::GetAcceleration()
{
	return m_Acceleration;
}

glm::vec3 Physics::GetForce()
{
    return m_Force;
}

glm::vec3 Physics::GetAngularVelocity()
{
	return m_AngularVelocity;
}

glm::mat3 Physics::GetInertiaTensor()
{
	glm::mat3 L_inverse = glm::mat3(0.0f);

	if(m_InverseInertiaTensor != L_inverse)
	{
		L_inverse = glm::inverse(m_InverseInertiaTensor);
	}

	return L_inverse;
}

glm::mat3 Physics::GetInverseInertiaTensor()
{
	return m_InverseInertiaTensor;
}

float Physics::GetFriction()
{
	return m_Friction;
}

float Physics::GetMass()
{
	return m_Mass;
}

float Physics::GetRestitution()
{
	return m_Restitution;
}

float Physics::GetLinearDamping()
{
	return m_LinearDamping;
}

float Physics::GetAngularDamping()
{
	return m_AngularDamping;
}

std::weak_ptr<CollisionShape> Physics::GetShape()
{
    return std::weak_ptr<CollisionShape>(m_Shape);
}

std::weak_ptr<Transform> Physics::GetTransform()
{
	return m_Transform;
}