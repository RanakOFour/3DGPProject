#include "GLPipeline/Entity/Collider/BoxCollider.h"
#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Collider/MeshCollider.h"
#include "GLPipeline/Entity/Model.h"
#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Size.h"

#include <glm/ext.hpp>
#include <memory>
#include "glm/ext.hpp"

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

Size& BoxCollider::GetSize()
{
	return m_Size;
}

bool BoxCollider::IsColliding(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	glm::vec3 L_pos = _transform->Position();
	glm::vec3 L_otherPos = _otherTransform->Position();
	glm::vec3 L_halfSize = glm::vec3(m_Size.Width() * 0.5f, m_Size.Height() * 0.5f, m_Size.Depth() * 0.5f);

	if(_otherCollider->m_type == ColliderType::Box)
	{
		BoxCollider* L_otherBox = (BoxCollider*)_otherCollider;
		glm::vec3 L_othHalfSize = glm::vec3(L_otherBox->GetSize().Width() / 2.0f, L_otherBox->GetSize().Height() / 2.0f, L_otherBox->GetSize().Depth() / 2.0f);

		// Big debug yippee
		printf("	AABB Pos: %f, %f, %f\n	AABBOPos: %f, %f, %f\n	AABB Siz: %f, %f, %f\n	AABBOSiz: %f, %f, %f\n\n",
		   L_pos.x, L_pos.y, L_pos.z,
		   L_otherPos.x, L_otherPos.y, L_otherPos.z,
		   L_halfSize.x, L_halfSize.y, L_halfSize.z,
		   L_othHalfSize.x, L_othHalfSize.y, L_othHalfSize.z);

		if (L_pos.x > L_otherPos.x)
		{
			if (L_otherPos.x + L_othHalfSize.x < L_pos.x - L_halfSize.x)
			{	
				printf("False 1\n");
				return false;
			}
		}
		else
		{
			if (L_pos.x + L_halfSize.x < L_otherPos.x - L_othHalfSize.x)
			{
				printf("False 2\n");
				return false;
			}
		}

		if (L_pos.y > L_otherPos.y)
		{
			if (L_otherPos.y + L_othHalfSize.y < L_pos.y - L_halfSize.y)
			{
				printf("False 3\n");
				return false;
			}
		}
		else
		{
			if (L_pos.y + L_halfSize.y < L_otherPos.y - L_othHalfSize.y)
			{
				printf("False 4\n");
				return false;
			}
		}

		if (L_pos.z > L_otherPos.z)
		{
			if (L_otherPos.z + L_othHalfSize.z < L_pos.z - L_halfSize.z)
			{
				printf("False 5\n");
				return false;
			}
		}
		else
		{
			if (L_pos.z + L_halfSize.z < L_otherPos.z - L_othHalfSize.z)
			{
				printf("False 6\n");
				return false;
			}
		}


		printf("True\n");
		return true;
	}
	else
	{
		MeshCollider* L_otherMesh = (MeshCollider*)_otherCollider;

		printf("BoxOnMesh\n");
		// Rewriting the collision is way too much work
		return L_otherMesh->IsColliding(_otherTransform, (Collider*)this, _transform);
	}
	
	printf("ERROR: Box Collider IsColliding normal exit!\n");
	return false;
}

glm::vec3 BoxCollider::HalfSize()
{
	glm::vec3 L_return = glm::vec3(
		m_Size.Width() * 0.5f,
		m_Size.Height() * 0.5f,
		m_Size.Depth() * 0.5f	
	);

	return L_return;
}

void BoxCollider::PrintInfo()
{
	printf("ColliderInfo:\n	Transform Pos: %f, %f, %f\n	Collider Type: %d\n", m_Transform->Position().x, m_Transform->Position().y, m_Transform->Position().z, m_type);
}