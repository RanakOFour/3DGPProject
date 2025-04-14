#ifndef MESHCOLLIDER_H

#define MESHCOLLIDER_H

#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Model.h"

#include <vector>

class MeshCollider : public Collider
{
    std::vector<Face> m_Triangles;
    public:
    MeshCollider(Transform* _parentTransform, std::vector<Face> _triangles);
    ~MeshCollider();

    bool IsColliding(Transform* _transform, Collider* _other, Transform* _otherTransform);

    inline std::vector<Face>& GetFaces()
    {
        return m_Triangles;
    }
};

#endif