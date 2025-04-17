#include "GLPipeline/Entity/Collider/MeshCollider.h"
#include "GLPipeline/Entity/Collider/BoxCollider.h"
#include "GLPipeline/Entity/Model.h"
#include <vector>

MeshCollider::MeshCollider(Transform* _parentTransform, std::vector<Face> _face) :
Collider(_parentTransform, ColliderType::Mesh),
m_Triangles(_face)
{
    for(int i = 0; i < m_Triangles.size(); ++i)
    {
        m_Triangles[i].CalculateNormal();
    } 
}

MeshCollider::~MeshCollider()
{

}

bool MeshCollider::IsColliding(Transform* _transform, Collider* _other, Transform* _otherTransform)
{
    glm::vec3 L_pos = _transform->Position();
	glm::vec3 L_otherPos = _otherTransform->Position();


    // May need to invert this
    if(_other->m_type == ColliderType::Box)
    {
        BoxCollider* L_boxOther = (BoxCollider*)_other;
        glm::vec3 L_othHalfSize = L_boxOther->HalfSize();
		
		printf("MeshOnBox:\n");
        for(int i = 0; i < m_Triangles.size(); ++i)
		{
			Face L_currentFace;

			// Create a triangle centered on the AABB of this box
			L_currentFace.a.position = m_Triangles[i].a.position + L_otherPos - L_pos;
			L_currentFace.b.position = m_Triangles[i].b.position + L_otherPos - L_pos;
			L_currentFace.c.position = m_Triangles[i].c.position + L_otherPos - L_pos;

			glm::vec3 L_triEdges[3] = {
				L_currentFace.b.position - L_currentFace.a.position,
				L_currentFace.c.position - L_currentFace.b.position,
				L_currentFace.a.position - L_currentFace.c.position
			};

			//printf("FaceInfo:\nPosA:%f, %f, %f\nPosB:%f, %f, %f\nPosC:%f, %f, %f\n", 
			//	   L_currentFace.a.position.x, L_currentFace.a.position.y, L_currentFace.a.position.z,
			//	   L_currentFace.b.position.x, L_currentFace.b.position.y, L_currentFace.b.position.z,
			//	   L_currentFace.c.position.x, L_currentFace.c.position.y, L_currentFace.c.position.z
			//	);

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
					if((L_currentAxis.x == 0.0f && L_currentAxis.y == 0.0f) && L_currentAxis.z == 0.0f)
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

					float L_boxRadius = L_othHalfSize.length();

					if(!(L_pointMin > L_boxRadius || L_pointMax < -L_boxRadius))
					{
						return true;
					}
				}
			}

			// Overlap in x y and z
			// There must be a better way to do this
			float L_min, L_max;
			L_min = glm::min(L_currentFace.a.position.x, L_currentFace.b.position.x, L_currentFace.c.position.x);
			L_max = glm::max(L_currentFace.a.position.x, L_currentFace.b.position.x, L_currentFace.c.position.x);
			
			if(!(L_min > L_othHalfSize.x || L_max < -L_othHalfSize.x))
			{
				return true;
			}


			L_min = glm::min(L_currentFace.a.position.y, L_currentFace.b.position.y, L_currentFace.c.position.y);
			L_max = glm::max(L_currentFace.a.position.y, L_currentFace.b.position.y, L_currentFace.c.position.y);

			if(!(L_min > L_othHalfSize.y || L_max < -L_othHalfSize.y))
			{
				return true;
			}


			L_min = glm::min(L_currentFace.a.position.z, L_currentFace.b.position.z, L_currentFace.c.position.z);
			L_max = glm::max(L_currentFace.a.position.z, L_currentFace.b.position.z, L_currentFace.c.position.z);

			if(!(L_min > L_othHalfSize.z || L_max < -L_othHalfSize.z))
			{
				return true;
			}

			// Test triangle normal

			glm::vec3 L_normal = glm::cross(L_triEdges[0], L_triEdges[1]);
			float L_dotProd = glm::dot(L_normal, L_currentFace.a.position);
			float L_radius = L_othHalfSize.x * fabs(L_normal.x) +
							 L_othHalfSize.y * fabs(L_normal.y) +
							 L_othHalfSize.z * fabs(L_normal.z);
							 
			if(L_dotProd < L_radius) return true;

            // L_currentFace.CalculateNormal();
            // _normalTotal += L_currentFace.normal;
			printf("Not Colliding\n");
			return false;
		}
    }
    else
    {
		printf("MeshOnMesh\n");
        return false;
    }

    printf("ERROR: MeshCollider::IsColliding normal exit!\n");
    return false;
}

std::vector<Face>& MeshCollider::GetFaces()
{
    return m_Triangles;
}