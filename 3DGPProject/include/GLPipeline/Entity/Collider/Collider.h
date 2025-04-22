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

	bool AxisTest(glm::vec3 _axis, Face& _face, glm::vec3& _boxHalfSize);
	bool TriangleBoxIntersect(Face& _face, glm::vec3& _halfLength);
	virtual bool IsColliding(Transform* _transform, Collider* _other, Transform* _otherTransform);
	void CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform);

	Transform* GetTransform();

	virtual void PrintInfo();
};

#endif