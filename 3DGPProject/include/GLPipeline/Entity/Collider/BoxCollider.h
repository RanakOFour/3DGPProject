#ifndef BOXCOLLIDER_H

#define BOXCOLLIDER_H

#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Size.h"

#include <memory>

class BoxCollider
{
private:
	std::shared_ptr<Transform> m_Transform;
	Size m_Size;

public:
	BoxCollider(Transform* _parentTransform);
	BoxCollider(Transform* _parentTransform, glm::vec3 _sizeDimensions);
	~BoxCollider();

	bool IsCollidingAABB(BoxCollider* _other);
	Transform* GetTransform();
	Size* GetSize();
};


#endif