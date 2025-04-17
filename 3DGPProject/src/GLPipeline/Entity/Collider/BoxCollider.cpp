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
		std::vector<Face>& L_faces = L_otherMesh->GetFaces();

		for(int i = 0; i < L_faces.size(); ++i)
		{
			Face L_currentFace;

			// Create a triangle centered on the AABB of this box
			L_currentFace.a.position = L_faces[i].a.position - L_pos;
			L_currentFace.b.position = L_faces[i].b.position - L_pos;
			L_currentFace.c.position = L_faces[i].c.position - L_pos;

			L_currentFace.CalculateNormal();

			glm::vec3 L_triEdges[3] = {
				L_currentFace.b.position - L_currentFace.a.position,
				L_currentFace.c.position - L_currentFace.b.position,
				L_currentFace.a.position - L_currentFace.c.position
			};

			glm::vec3 L_axis[3] = {
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			// Test 9 cross axis
			for(int j = 0; j < 3; ++j)
			{
				for(int k = 0; k < 3; ++k)
				{
					glm::vec3 L_currentAxis = glm::cross(L_triEdges[j], L_axis[k]);
					
					// Ignore zero axis
					if(L_currentAxis.length() == 0.0f)
					{
						continue;
					}

					glm::vec3 L_points = {
						glm::dot(L_currentFace.a.position, L_currentAxis),
						glm::dot(L_currentFace.b.position, L_currentAxis),
						glm::dot(L_currentFace.c.position, L_currentAxis)
					};

					float L_pointMin = glm::min(L_points.x, L_points.y, L_points.z);
					float L_pointMax = glm::max(L_points.x, L_points.y, L_points.z);

					float L_boxRadius = L_halfSize.x * L_currentAxis.x +
										L_halfSize.y * L_currentAxis.y +
										L_halfSize.z * L_currentAxis.z;

					if(L_pointMin > L_boxRadius || L_pointMax < -L_boxRadius)
					{
						return false;
					}
				}
			}

			// Overlap in x y and z
			// There must be a better way to do this
			float L_min, L_max;
			L_min = glm::min(L_currentFace.a.position.x, L_currentFace.b.position.x, L_currentFace.c.position.x);
			L_max = glm::max(L_currentFace.a.position.x, L_currentFace.b.position.x, L_currentFace.c.position.x);
			
			if(L_min > L_halfSize.x || L_max < -L_halfSize.x)
			{
				return false;
			}


			L_min = glm::min(L_currentFace.a.position.y, L_currentFace.b.position.y, L_currentFace.c.position.y);
			L_max = glm::max(L_currentFace.a.position.y, L_currentFace.b.position.y, L_currentFace.c.position.y);

			if(L_min > L_halfSize.y || L_max < -L_halfSize.y)
			{
				return false;
			}


			L_min = glm::min(L_currentFace.a.position.z, L_currentFace.b.position.z, L_currentFace.c.position.z);
			L_max = glm::max(L_currentFace.a.position.z, L_currentFace.b.position.z, L_currentFace.c.position.z);

			if(L_min > L_halfSize.z || L_max < -L_halfSize.z)
			{
				return false;
			}

			// Test triangle normal

			glm::vec3 L_normal = glm::cross(L_triEdges[0], L_triEdges[1]);
			float L_dotProd = glm::dot(L_normal, L_currentFace.a.position);
			float L_radius = L_halfSize.x * fabs(L_normal.x) +
							 L_halfSize.y * fabs(L_normal.y) +
							 L_halfSize.z * fabs(L_normal.z);
							 
			if(L_dotProd > L_radius) return false;

			return true;
		}
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