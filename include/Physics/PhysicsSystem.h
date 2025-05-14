#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "Physics/Transform.h"
#include "Physics/Physics.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/SphereShape.h"
#include "Physics/Shape/MeshShape.h"

#include "glm/ext.hpp"

#include <memory>

namespace PhysicsSystem
{

struct ContactPoint
{
    // Local position of contact point from A
    glm::vec3 localA;

    // Local position of contact point from B
    glm::vec3 localB;

    // Normal to contact point from B to A
    glm::vec3 normal;

    // Distance overlapped into object A (should be same for B)
    float penetration;
};

class CollisionInfo
{
    public:
    CollisionInfo() {};
    ~CollisionInfo() {};

    // Raw is fine as CollisionInfo should not be made if 2 colliding objects do not exist
    Physics* objectA;
    Physics* objectB;

    ContactPoint contactPoint;

    void AddContactPoint(glm::vec3 _localA, glm::vec3 _localB, glm::vec3 _normal, float _penetration)
    {
        contactPoint.localA = _localA;
        contactPoint.localB = _localB;
        contactPoint.normal = _normal;
        contactPoint.penetration = _penetration;
    }
};

class CollisionDetection
{   
    static bool OBBCollision(CubeShape* _cubeA, Transform* _transformA, CubeShape* _cubeB, Transform* _transformB, CollisionInfo* _infoOut);
    static bool OBBvsSphereCollision(CubeShape* _cube, Transform* _cubeTransform, SphereShape* _sphere, Transform* _sphereTransform, CollisionInfo* _infoOut);
    static bool SphereCollision(SphereShape* _sphereA, Transform* _transformA, SphereShape* _sphereB, Transform* _transformB, CollisionInfo* _infoOut);
    static bool MeshCollision(MeshShape* _meshA, Transform* _transformA, MeshShape* _meshB, Transform* _transformB, CollisionInfo* _infoOut);
    static bool MeshCubeCollision(MeshShape* _mesh, Transform* _meshTransform, CubeShape* _cube, Transform* _cubeTransform, CollisionInfo* _infoOut);
    
    public:

    static bool CollisionCheck(Physics& _aObject, Physics& _bObject, CollisionInfo* _infoOut);
    static void ImpulseCollisionResolution(CollisionInfo* _info);
};
}

#endif