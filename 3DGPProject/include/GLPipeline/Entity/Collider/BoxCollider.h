#ifndef BOXCOLLIDER_H

#define BOXCOLLIDER_H

#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Size.h"

#include <memory>

class BoxCollider : Collider
{
private:
	Size m_Size;

public:
	BoxCollider(Transform* _parentTransform);
	BoxCollider(Transform* _parentTransform, glm::vec3 _sizeDimensions);
	~BoxCollider();

	bool IsCollidingAABB(Transform* _transform, BoxCollider* _otherCollider, Transform* _otherTransform);
	void CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform);
	Size& GetSize();

	void PrintInfo();
};


#endif