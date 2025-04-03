#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Size.h"
#include "GLPipeline/Entity/Collider/BoxCollider.h"

#include <glm/ext.hpp>
#include <memory>

BoxCollider::BoxCollider(Transform* _parentTransform) :
	m_Transform(),
	m_Size()
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
}

BoxCollider::BoxCollider(Transform* _parentTransform, glm::vec3 _sizeDimensions) :
	m_Transform(nullptr),
	m_Size(_sizeDimensions, glm::vec3(0.0f, 0.0f, 0.0f))
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
}

BoxCollider::~BoxCollider()
{

}

Transform* BoxCollider::GetTransform()
{
	return m_Transform.get();
}

Size* BoxCollider::GetSize()
{
	return &m_Size;
}

bool BoxCollider::IsCollidingAABB(BoxCollider* _other)
{
	glm::vec3 L_pos = L_pos;
	glm::vec3 L_otherPos = _other->GetTransform()->Position();
	Size* L_othSize = _other->GetSize();

	if (L_pos.x > L_otherPos.x)
	{
		if (L_otherPos.x + (L_othSize->Width() * 0.5f) < L_pos.x + (m_Size.Width() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.x + (m_Size.Width() * 0.5f) < L_otherPos.x + (L_othSize->Width() * 0.5f))
		{
			return false;
		}
	}

	if (L_pos.y > L_otherPos.y)
	{
		if (L_otherPos.y + (L_othSize->Height() * 0.5f) < L_pos.y + (m_Size.Height() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.y + (m_Size.Height() * 0.5f) < L_otherPos.y + (L_othSize->Height() * 0.5f))
		{
			return false;
		}
	}

	if (L_pos.z > L_otherPos.z)
	{
		if (L_otherPos.z + (L_othSize->Depth() * 0.5f) < L_pos.z + (m_Size.Depth() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.z + (m_Size.Depth() * 0.5f) < L_otherPos.z + (L_othSize->Depth() * 0.5f))
		{
			return false;
		}
	}
	

	return true;
}