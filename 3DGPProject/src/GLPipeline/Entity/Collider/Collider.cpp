#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"

#include <memory>
#include <iostream>

Collider::Collider(Transform* _parentTransform,	ColliderType _type)
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
	m_type = _type;
}

Collider::~Collider()
{

}

void Collider::CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	glm::vec3& L_pos = _transform->Position();
	glm::vec3& L_otherPos = _otherTransform->Position();

	float L_ammount = 0.1f;
	float L_step = 0.1f;

	while (true)
	{
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.x += L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.x -= L_ammount;
		L_pos.x -= L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.x += L_ammount;
		L_pos.z += L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.z -= L_ammount;
		L_pos.z -= L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.z += L_ammount;
		L_pos.y += L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.y -= L_ammount;
		L_pos.y -= L_ammount;
		printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
		if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
		L_pos.y += L_ammount;
		L_ammount += L_step;
	}
}

Transform* Collider::GetTransform()
{
	return m_Transform.get();
}

void Collider::PrintInfo()
{
	printf("ColliderInfo:\n	Transform Pos: %f, %f, %f\n	Collider Type: %d\n", m_Transform->Position().x, m_Transform->Position().y, m_Transform->Position().z, m_type);
}