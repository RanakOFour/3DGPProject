#ifndef MESHCOLLIDER_H

#define MESHCOLLIDER_H

#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Model.h"

#include <vector>

class MeshCollider : public Collider
{
    std::vector<Face> m_Triangles;
    std::vector<Face*> m_CollidingFaces;
    public:
    MeshCollider(Transform* _parentTransform, std::vector<Face> _triangles);
    ~MeshCollider();

    bool AxisTest(glm::vec3 axis, Face& _face, glm::vec3& boxHalfSize);
    bool TriangleBoxIntersect(Face& _face, glm::vec3& _boxHalfSize);
    bool IsColliding(Transform* _transform, Collider* _other, Transform* _otherTransform);
    std::vector<Face>& GetFaces();
    std::vector<Face*>& GetCollidingFaces();
};

#endif