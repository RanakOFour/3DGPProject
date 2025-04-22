#include "GLPipeline/Entity/Collider/MeshCollider.h"
#include "GLPipeline/Entity/Collider/BoxCollider.h"
#include "GLPipeline/Entity/Model.h"
#include <vector>

MeshCollider::MeshCollider(Transform* _parentTransform, std::vector<Face> _face) :
Collider(_parentTransform, ColliderType::Mesh),
m_Triangles(_face),
m_CollidingFaces()
{
    for(int i = 0; i < m_Triangles.size(); ++i)
    {
        m_Triangles[i].CalculateNormal();
    } 
}

MeshCollider::~MeshCollider()
{

}

bool MeshCollider::AxisTest(glm::vec3 axis, Face& _face, glm::vec3& boxHalfSize) 
{
	if (glm::length(axis) < 1e-6f){ return true; } // Skip near-zero axis

    glm::vec3 L_normAxis = glm::normalize(axis);

    float p0 = glm::dot(_face.a.position, L_normAxis);
    float p1 = glm::dot(_face.b.position, L_normAxis);
    float p2 = glm::dot(_face.c.position, L_normAxis);

    float minP = glm::min(p0, p1, p2);
    float maxP = glm::max(p0, p1, p2);

    float r = boxHalfSize.x * fabs(L_normAxis.x) +
              boxHalfSize.y * fabs(L_normAxis.y) +
              boxHalfSize.z * fabs(L_normAxis.z);

    return !(minP > r || maxP < -r);
}

bool MeshCollider::TriangleBoxIntersect(Face& _face, glm::vec3& _boxHalfSize) 
{
	glm::vec3 boxAxis[3] = { glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1) };

    glm::vec3 edges[3] = { _face.b.position - _face.a.position,
								 _face.c.position - _face.b.position,
								 _face.a.position - _face.c.position };

    // Test box normals
    for (int i = 0; i < 3; ++i)
	{
        if (!AxisTest(boxAxis[i], _face, _boxHalfSize)){ return false; }
    }

    // Test triangle normal
    glm::vec3 L_triNormal = glm::cross(edges[0], edges[1]);
    if (!AxisTest(L_triNormal, _face, _boxHalfSize)){ return false; }

    // Test edge cross axes
    for (int i = 0; i < 3; ++i) 
	{
        for (int j = 0; j < 3; ++j) 
		{
            glm::vec3 L_testAxis = glm::cross(edges[i], boxAxis[j]);
            if (!AxisTest(L_testAxis, _face, _boxHalfSize)){ return false; }
        }
    }

    return true;
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
		
		printf("MeshOnBox:\nPos : %f, %f, %f\n PosB: %f, %f, %f\n",
		L_pos.x, L_pos.y, L_pos.z,
		L_otherPos.x, L_otherPos.y, L_otherPos.z);
		
		for(int i = 0; i < m_Triangles.size(); i++)
		{
			Face L_currentFace = m_Triangles[i];

            L_currentFace.a.position += L_pos - L_otherPos;
            L_currentFace.b.position += L_pos - L_otherPos;
            L_currentFace.c.position += L_pos - L_otherPos;

			if(TriangleBoxIntersect(L_currentFace, L_othHalfSize))
			{
                // The normals of the triangles are already calculated, and should be the same as L_currentFace
                m_CollidingFaces.push_back(&m_Triangles[i]);
			}
		}

        printf("Mesh not colliding with box\n");
        return false;
    }
    else
    {
		printf("MeshOnMesh\n");
        return false;
    }
}

std::vector<Face>& MeshCollider::GetFaces()
{
    return m_Triangles;
}

std::vector<Face*>& MeshCollider::GetCollidingFaces()
{
    return m_CollidingFaces;
}