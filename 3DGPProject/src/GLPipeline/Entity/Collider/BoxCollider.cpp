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

bool BoxCollider::IsCollidingAABB(BoxCollider* _other)
{
	glm::vec3 L_pos = m_Transform->Position();
	glm::vec3 L_otherPos = _other->GetTransform()->Position();
	Size* L_otherSize = _other->GetSize();

	// Big debug yippee
	printf("AABB Pos: %f, %f, %f\nAABBOPos: %f, %f, %f\nAABB Siz: %f, %f, %f\nAABBOSiz: %f, %f, %f\n\n",
		   L_pos.x, L_pos.y, L_pos.z,
		   L_otherPos.x, L_otherPos.y, L_otherPos.z,
		   m_Size.Width(), m_Size.Height(), m_Size.Depth(),
		   L_otherSize->Width(), L_otherSize->Height(), L_otherSize->Depth());

	if (L_pos.x > L_otherPos.x)
	{
		if (L_otherPos.x + (L_otherSize->Width() * 0.5f) < L_pos.x + (m_Size.Width() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.x + (m_Size.Width() * 0.5f) < L_otherPos.x + (L_otherSize->Width() * 0.5f))
		{
			return false;
		}
	}

	if (L_pos.y > L_otherPos.y)
	{
		if (L_otherPos.y + (L_otherSize->Height() * 0.5f) < L_pos.y + (m_Size.Height() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.y + (m_Size.Height() * 0.5f) < L_otherPos.y + (L_otherSize->Height() * 0.5f))
		{
			return false;
		}
	}

	if (L_pos.z > L_otherPos.z)
	{
		if (L_otherPos.z + (L_otherSize->Depth() * 0.5f) < L_pos.z + (m_Size.Depth() * 0.5f))
		{
			return false;
		}
	}
	else
	{
		if (L_pos.z + (m_Size.Depth() * 0.5f) < L_otherPos.z + (L_otherSize->Depth() * 0.5f))
		{
			return false;
		}
	}

	return true;
}

void BoxCollider::CollisionResponse(Collider* _other)
{
	glm::vec3 L_pos = m_Transform->Position();

	if (_other->type == ColliderType::Box)
	{
		BoxCollider* _otherBox = (BoxCollider*)_other;

		float L_ammount = 0.1f;
		float L_step = 0.1f;

		while (true)
		{
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.x += L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.x -= L_ammount;
			L_pos.x -= L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.x += L_ammount;
			L_pos.z += L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.z -= L_ammount;
			L_pos.z -= L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.z += L_ammount;
			L_pos.y += L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.y -= L_ammount;
			L_pos.y -= L_ammount;
			if (!IsCollidingAABB(_otherBox)) break;
			L_pos.y += L_ammount;

			L_ammount += L_step;
		}
	}
	else
	{ //Mesh collision
		
	}
}