#ifndef COLLIDER_H

#define COLLIDER_H

#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Model.h"

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
	
	ColliderType m_type;
	
	Collider(Transform* _parentTransform, ColliderType _type);
	~Collider();
	virtual bool IsColliding(Transform* _transform, Collider* _other, Transform* _otherTransform);
	void CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform);

	Transform* GetTransform();

	virtual void PrintInfo();
};

#endif