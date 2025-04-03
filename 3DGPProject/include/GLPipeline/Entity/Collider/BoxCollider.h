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

	bool IsCollidingAABB(Collider* _other);
	void CollisionResponse(Collider* _other);
	Size* GetSize();
};


#endif