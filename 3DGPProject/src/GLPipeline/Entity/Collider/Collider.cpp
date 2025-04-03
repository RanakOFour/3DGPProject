#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"

#include <memory>
#include <iostream>

Collider::Collider(Transform* _parentCollider,	ColliderType _type)
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
	type = _type;
}

Collider::~Collider()
{

}

bool Collider::IsCollidingAABB(Collider* _other)
{
	printf("BASE COLLIDER CALL IS BAD!\n");
	return false;
}

void Collider::CollisionResponse(Collider* _other)
{
	printf("BASE COLLIDER CALL IS BAD!\n");
}

Transform* Collider::GetTransform()
{
	return &m_Transform;
}