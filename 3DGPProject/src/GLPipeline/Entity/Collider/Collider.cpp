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

void Collider::CollisionResponse(Collider* _other)
{
	printf("BASE COLLIDER CALL IS BAD!\n");
}

Transform* Collider::GetTransform()
{
	return m_Transform.get();
}