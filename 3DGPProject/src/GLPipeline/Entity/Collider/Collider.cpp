#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"

#include <memory>
#include <iostream>

Collider::Collider(Transform* _parentTransform,	ColliderType _type)
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
	type = _type;
}

Collider::~Collider()
{

}

void Collider::CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	printf("BASE COLLIDER CALL IS BAD!\n");
}

Transform* Collider::GetTransform()
{
	return m_Transform.get();
}

void Collider::PrintInfo()
{
	printf("ColliderInfo:\n	Transform Pos: %f, %f, %f\n	Collider Type: %d\n", m_Transform->Position().x, m_Transform->Position().y, m_Transform->Position().z, type);
}