#ifndef COLLIDER_H

#define COLLIDER_H

#include "GLPipeline/Entity/Transform.h"

#include <memory>

class Collider
{
protected:
	std::shared_ptr<Transform> m_Transform;

public:
	enum ColliderType
	{
		Box,
		Mesh
	};
	
	ColliderType type;
	
	Collider(Transform* _parentTransform, ColliderType _type);
	~Collider();

	virtual void CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform);

	Transform* GetTransform();

	void PrintInfo();
};

#endif