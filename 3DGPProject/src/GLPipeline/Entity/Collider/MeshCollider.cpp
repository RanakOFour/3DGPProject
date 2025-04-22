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
		
		printf("MeshOnBox:\nPos : %f, %f, %f\n PosB: %f, %f, %f\n",
		L_pos.x, L_pos.y, L_pos.z,
		L_otherPos.x, L_otherPos.y, L_otherPos.z);
		
		for(int i = 0; i < m_Triangles.size(); i++)
		{
			Face L_currentFace = m_Triangles[i];

            L_currentFace.a.position += L_pos;
            L_currentFace.b.position += L_pos;
            L_currentFace.c.position += L_pos;

			if(TriangleBoxIntersect(L_currentFace, L_othHalfSize))
			{
				return true;
			}
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