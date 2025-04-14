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

    if(_other->type == ColliderType::Box)
    {
        BoxCollider* L_boxOther = (BoxCollider*)_other;

    }
    else
    {
        
    }
}