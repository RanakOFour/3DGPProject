#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Size.h"
#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Collider/BoxCollider.h"

#include <glm/ext.hpp>
#include <memory>

BoxCollider::BoxCollider(Transform* _parentTransform) : 
	Collider(_parentTransform, ColliderType::Box),
	m_Size()
{
}

BoxCollider::BoxCollider(Transform* _parentTransform, glm::vec3 _sizeDimensions) :
	Collider(_parentTransform, ColliderType::Box),
	m_Size(_sizeDimensions, glm::vec3(0.0f, 0.0f, 0.0f))
{
}

BoxCollider::~BoxCollider()
{

}

Size* BoxCollider::GetSize()
{
	return &m_Size;
}

bool BoxCollider::IsCollidingAABB(Collider* _other)
{
	glm::vec3 L_pos = *m_Transform->Position();
	glm::vec3 L_otherPos = *_other->GetTransform()->Position();
	Size* L_othSize;
	if (_other->type == ColliderType::Box)
	{
		BoxCollider* _otherBox = (BoxCollider*)_other;
		L_othSize = _otherBox->GetSize();
	}

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

void BoxCollider::CollisionResponse(Collider* _other)
{
	glm::vec3 L_pos = *m_Transform->Position();
	glm::vec3 L_otherPos = *_other->GetTransform()->Position();
	Size* L_othSize;
	if (_other->type == ColliderType::Box)
	{
		BoxCollider* _otherBox = (BoxCollider*)_other;
		L_othSize = _otherBox->GetSize();
	}

	float L_ammount = 0.1f;
	float L_step = 0.1f;

	while (true)
	{
		if (!IsCollidingAABB(_other)) break;
		L_pos.x += L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.x -= L_ammount;
		L_pos.x -= L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.x += L_ammount;
		L_pos.z += L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.z -= L_ammount;
		L_pos.z -= L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.z += L_ammount;
		L_pos.y += L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.y -= L_ammount;
		L_pos.y -= L_ammount;
		if (!IsCollidingAABB(_other)) break;
		L_pos.y += L_ammount;

		L_ammount += L_step;
	}
}