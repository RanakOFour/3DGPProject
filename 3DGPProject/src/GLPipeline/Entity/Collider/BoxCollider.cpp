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

Size& BoxCollider::GetSize()
{
	return m_Size;
}

bool BoxCollider::IsCollidingAABB(Transform* _transform, BoxCollider* _otherCollider, Transform* _otherTransform)
{
	glm::vec3 L_pos = _transform->Position();
	glm::vec3 L_otherPos = _otherTransform->Position();
	Size L_otherSize = _otherCollider->GetSize();

	glm::vec3 L_halfSize = glm::vec3(m_Size.Width() / 2.0f, m_Size.Height() / 2.0f, m_Size.Depth() / 2.0f);
	glm::vec3 L_othHalfSize = glm::vec3(_otherCollider->GetSize().Width() / 2.0f, _otherCollider->GetSize().Height() / 2.0f, _otherCollider->GetSize().Depth() / 2.0f);

	// Big debug yippee
	printf("AABB Pos: %f, %f, %f\nAABBOPos: %f, %f, %f\nAABB Siz: %f, %f, %f\nAABBOSiz: %f, %f, %f\n\n",
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

void BoxCollider::CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	glm::vec3& L_pos = _transform->Position();
	glm::vec3& L_otherPos = _otherTransform->Position();

	if (_otherCollider->type == ColliderType::Box)
	{
		BoxCollider* _otherBox = (BoxCollider*)_otherCollider;

		float L_ammount = 0.1f;
		float L_step = 0.1f;

		while (true)
		{
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.x += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.x -= L_ammount;
			L_pos.x -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.x += L_ammount;
			L_pos.z += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.z -= L_ammount;
			L_pos.z -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.z += L_ammount;
			L_pos.y += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.y -= L_ammount;
			L_pos.y -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsCollidingAABB(_transform, _otherBox, _otherTransform)){ break;}
			L_pos.y += L_ammount;

			L_ammount += L_step;
		}
	}
	else
	{ //Mesh collision
		
	}
}

void BoxCollider::PrintInfo()
{
	printf("ColliderInfo:\n	Transform Pos: %f, %f, %f\n	Collider Type: %d\n", m_Transform->Position().x, m_Transform->Position().y, m_Transform->Position().z, type);
}