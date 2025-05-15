
#include "Physics/PhysicsSystem.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/SphereShape.h"

#include "glm/ext.hpp"

bool PhysicsSystem::CollisionDetection::OBBCollision(CubeShape* _cubeA, Transform* _transformA, CubeShape* _cubeB, Transform* _transformB, CollisionInfo* _infoOut)
{
    //printf("AABB Collision\n");
    glm::vec3 L_delta = _transformB->GetPosition() - _transformA->GetPosition();
    glm::vec3 L_totalSize = _cubeA->GetHalfSize() + _cubeB->GetHalfSize();

    if(glm::abs(L_delta.x) < L_totalSize.x &&
       glm::abs(L_delta.y) < L_totalSize.y &&
       glm::abs(L_delta.z) < L_totalSize.z)
    {
        glm::vec3 L_posA = _transformA->GetPosition();
        glm::vec3 L_posB = _transformB->GetPosition();

        glm::vec3 L_boxHalfA = _cubeA->GetHalfSize();
        glm::vec3 L_boxHalfB = _cubeB->GetHalfSize();

        glm::vec3 L_faces[6] = {
            {-1, 0, 0}, {1, 0, 0},
            {0, -1, 0}, {0, 1, 0},
            {0, 0, -1}, {0, 0, 1}
        };

        glm::vec3 L_maxA = L_posA + L_boxHalfA;
        glm::vec3 L_minA = L_posA - L_boxHalfA;

        glm::vec3 L_maxB = L_posB + L_boxHalfB;
        glm::vec3 L_minB = L_posB - L_boxHalfB;

        float L_distance[6]
        {
            (L_maxB.x - L_minA.x),
            (L_maxA.x - L_minB.x),
            (L_maxB.y - L_minA.y),
            (L_maxA.y - L_minB.y),
            (L_maxB.z - L_minA.z),
            (L_maxA.z - L_minB.z)
        };

        float L_penetration = FLT_MAX;
        glm::vec3 L_bestAxis;

        for(int i = 0; i < 6; ++i)
        {
            if(L_distance[i] < L_penetration)
            {
                L_penetration = L_distance[i];
                L_bestAxis = L_faces[i];
            }
        }

        glm::vec3 L_localA = glm::vec3(0.0f);

        _infoOut->AddContactPoint(L_localA, L_localA, L_bestAxis, L_penetration);
        return true;
    }

    return false;
}

bool PhysicsSystem::CollisionDetection::OBBvsSphereCollision(CubeShape* _cube, Transform* _cubeTransform, SphereShape* _sphere, Transform* _sphereTransform, CollisionInfo* _infoOut)
{
    // Calculate sphere position in cube's local space
    glm::vec3 L_dist = _sphereTransform->GetPosition() - _cubeTransform->GetPosition();
    glm::vec3 L_sphereLocal = glm::vec3(
        glm::dot(_cubeTransform->Right(), L_dist),
        glm::dot(_cubeTransform->Up(), L_dist),
        glm::dot(_cubeTransform->Forward(), L_dist)
    );

    glm::vec3 L_cubeHalfSize = _cube->GetHalfSize();

    // Find closest point on cube to sphere
    glm::vec3 L_closestLocalPoint;
    for (int axis = 0; axis < 3; ++axis)
    {
        float val = L_sphereLocal[axis];
        val = glm::clamp(val, -L_cubeHalfSize[axis], L_cubeHalfSize[axis]);
        L_closestLocalPoint[axis] = val;
    }

    // Vector from closest point to sphere
    glm::vec3 L_localDifference = L_sphereLocal - L_closestLocalPoint;
    float L_distance = glm::length(L_localDifference);
    float L_penetration = _sphere->GetRadius() - L_distance;

    if (L_penetration <= 0) return false;

    glm::vec3 L_normalLocal;
    if (L_distance > 1e-3f)
    {
        L_normalLocal = glm::normalize(L_localDifference);
    }
    else
    {
        L_normalLocal = glm::normalize(L_sphereLocal);
    }

    glm::vec3 L_normal = 
        _cubeTransform->Right() * L_normalLocal.x +
        _cubeTransform->Up() * L_normalLocal.y +
        _cubeTransform->Forward() * L_normalLocal.z;
    L_normal = glm::normalize(L_normal);

    // Ensure normal points from A (cube) to B (sphere)
    if (glm::dot(L_normal, L_dist) < 0.0f)
    {
        L_normal = -L_normal;
    }

    // CORRECTED: Use '+' for all axes (removed '-' for Right())
    glm::vec3 L_worldContactA = _cubeTransform->GetPosition() +
        _cubeTransform->Right() * L_closestLocalPoint.x +
        _cubeTransform->Up() * L_closestLocalPoint.y +
        _cubeTransform->Forward() * L_closestLocalPoint.z;

    glm::vec3 L_worldContactB = _sphereTransform->GetPosition() - (L_normal * _sphere->GetRadius());

    // Convert to local space
    glm::mat3 L_rotCube = glm::mat3_cast(_cubeTransform->GetRotation());
    glm::mat3 L_rotSphere = glm::mat3_cast(_sphereTransform->GetRotation());
    glm::vec3 L_localA = glm::transpose(L_rotCube) * (L_worldContactA - _cubeTransform->GetPosition());
    glm::vec3 L_localB = glm::transpose(L_rotSphere) * (L_worldContactB - _sphereTransform->GetPosition());

    _infoOut->AddContactPoint(L_localA, L_localB, L_normal, L_penetration);
    return true;
}

bool PhysicsSystem::CollisionDetection::SphereCollision(SphereShape *_sphereA, Transform *_transformA, SphereShape *_sphereB, Transform *_transformB, CollisionInfo *_infoOut)
{
    glm::vec3 L_delta = _transformB->GetPosition() - _transformA->GetPosition();
    float L_distance = glm::length(L_delta);
    float L_radii = _sphereA->GetRadius() + _sphereB->GetRadius() + 0.01f;

    if (L_distance <= L_radii) 
    {
        float L_penetration = L_radii - L_distance;
        glm::vec3 L_normal = (L_distance > 1e-7f) ? glm::normalize(L_delta) : glm::vec3(0.0f, 1.0f, 0.0f);

        // Ensure normal points from A to B
        if (glm::dot(L_normal, L_delta) < 0.0f)
        {
            L_normal = -L_normal;
        }

        glm::vec3 L_worldContactA = _transformA->GetPosition() + L_normal * _sphereA->GetRadius();
        glm::vec3 L_worldContactB = _transformB->GetPosition() - L_normal * _sphereB->GetRadius();

        // Convert to local space
        glm::mat3 L_rotA = glm::mat3_cast(_transformA->GetRotation());
        glm::mat3 L_rotB = glm::mat3_cast(_transformB->GetRotation());
        glm::vec3 L_localA = glm::transpose(L_rotA) * (L_worldContactA - _transformA->GetPosition());
        glm::vec3 L_localB = glm::transpose(L_rotB) * (L_worldContactB - _transformB->GetPosition());

        _infoOut->AddContactPoint(L_localA, L_localB, L_normal, L_penetration);
        return true;
    }
    return false;
}


bool AxisTest(glm::vec3 axis, Face& _face, glm::vec3& boxHalfSize) 
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

bool TriangleBoxIntersect(Face& _face, glm::vec3& _boxHalfSize) 
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

bool PhysicsSystem::CollisionDetection::MeshCollision(MeshShape* _meshA, Transform* _transformA, MeshShape* _meshB, Transform* _transformB, CollisionInfo* _infoOut)
{
    //Beep boop
    return false;
}

bool PhysicsSystem::CollisionDetection::MeshCubeCollision(MeshShape* _mesh, Transform* _meshTransform, CubeShape* _cube, Transform* _cubeTransform, CollisionInfo* _infoOut)
{
    std::vector<Face> L_faces = _mesh->GetFaces();
    glm::vec3 L_halfSize = _cube->GetHalfSize();

    glm::vec3 L_totalNormal = glm::vec3(0.0f);
	printf("MeshOnBox\n");
		
	for(int i = 0; i < L_faces.size(); ++i)
	{
		Face L_currentFace = L_faces[i];
        L_currentFace.a.position += _meshTransform - _cubeTransform;
        L_currentFace.b.position += _meshTransform - _cubeTransform;
        L_currentFace.c.position += _meshTransform - _cubeTransform;
        L_currentFace.CalculateNormal();

		if(TriangleBoxIntersect(L_currentFace, L_halfSize))
		{
            // The normals of the triangles are already calculated, and should be the same as L_currentFace
            L_totalNormal += L_currentFace.normal;
		}
	}

    printf("Mesh not colliding with box\n");
    return false;
}

bool PhysicsSystem::CollisionDetection::CollisionCheck(CollisionShape* _aShape, CollisionShape* _bShape, CollisionInfo *_infoOut)
{
    _infoOut->objectA = _aShape;
    _infoOut->objectB = _bShape;

    Transform* L_transformA = _aShape->GetTransform().lock().get();
    Transform* L_transformB = _bShape->GetTransform().lock().get();

    int L_shapeTypeA = (int)_aShape->GetType();
    int L_shapeTypeB = (int)_bShape->GetType();

    ShapeType L_shapeType = (ShapeType)(((int)_aShape->GetType()) | ((int)_bShape->GetType()));

    if (L_shapeType == ShapeType::Cube)
    {
        return OBBCollision((CubeShape*)_aShape, L_transformA, (CubeShape*)_bShape, L_transformB, _infoOut);
    }
    else if (L_shapeType == ShapeType::Sphere)
    {
        return SphereCollision((SphereShape*)_aShape, L_transformA, (SphereShape*)_bShape, L_transformB, _infoOut);
    }
    else if(L_shapeType == ShapeType::Mesh)
    {
        return MeshCollision((MeshShape*)_aShape, L_transformA, (MeshShape*)_bShape, L_transformB, _infoOut);
    }
    
    if (_aShape->GetType() == ShapeType::Cube)
    {
        if(_bShape->GetType() == ShapeType::Sphere)
        {
            return OBBvsSphereCollision((CubeShape*)_aShape, L_transformA, (SphereShape*)_bShape, L_transformB, _infoOut);
        }
        else
        { // Mesh
            _infoOut->objectA = _bShape;
            _infoOut->objectB = _aShape;
            return MeshCubeCollision((MeshShape*)_bShape, L_transformB, (CubeShape*)_aShape, L_transformA, _infoOut);
        }
    }
    if (_aShape->GetType() == ShapeType::Sphere)
    {
        if(_bShape->GetType() == ShapeType::Cube)
        {
            _infoOut->objectA = _bShape;
            _infoOut->objectB = _aShape;
            return OBBvsSphereCollision((CubeShape*)_bShape, L_transformB, (SphereShape*)_aShape, L_transformA, _infoOut);
        }
        else
        { // Mesh

        }
    }
    if (_aShape->GetType() == ShapeType::Mesh)
    {
        if(_bShape->GetType() == ShapeType::Cube)
        {
            return MeshCubeCollision((MeshShape*)_aShape, L_transformA, (CubeShape*)_bShape, L_transformB, _infoOut);
        }
        else
        { //Sphere

        }
    }

    return false;
}

void PhysicsSystem::CollisionDetection::ImpulseCollisionResolution(CollisionInfo* _info)
{
    CollisionShape* L_aObject = _info->objectA;
    CollisionShape* L_bObject = _info->objectB;
    ContactPoint L_contact = _info->contactPoint;

    Transform* L_transformA = L_aObject->GetTransform().lock().get();
    Transform* L_transformB = L_bObject->GetTransform().lock().get();
    // Inverse masses
    if (L_aObject->Environment() && L_bObject->Environment()) return;


    glm::vec3 L_correction = L_contact.normal * L_contact.penetration;

    if(L_aObject->Environment())
    {
        L_transformB->Move(L_correction);
    }
    else if(L_bObject->Environment())
    {
        L_transformA->Move(-L_correction);
    }
    else
    {
        L_transformA->Move(-L_correction);
        L_transformB->Move(L_correction);
    }

    // Silly physics things I don't need
    // Newton died a virgin
    // Calculate torque arms in world space
    // glm::quat L_rotA = L_aObject->GetTransform().lock()->GetRotation();
    // glm::quat L_rotB = L_bObject->GetTransform().lock()->GetRotation();
    // glm::vec3 L_torqueArmA = L_rotA * L_contact.localA;
    // glm::vec3 L_torqueArmB = L_rotB * L_contact.localB;

    // // Relative velocity
    // glm::vec3 L_velA = L_aObject->GetVelocity() + glm::cross(L_aObject->GetAngularVelocity(), L_torqueArmA);
    // glm::vec3 L_velB = L_bObject->GetVelocity() + glm::cross(L_bObject->GetAngularVelocity(), L_torqueArmB);
    // glm::vec3 L_relVel = L_velB - L_velA;

    // // Normal impulse
    // float L_velAlongNormal = glm::dot(L_relVel, L_contact.normal);
    // if (L_velAlongNormal > 0) return;

    // // Rotate inertia tensors to world space
    // glm::mat3 L_rotMatA = glm::mat3_cast(L_rotA);
    // glm::mat3 L_rotMatB = glm::mat3_cast(L_rotB);
    // glm::mat3 L_invInertiaA = L_rotMatA * L_aObject->GetInverseInertiaTensor() * glm::transpose(L_rotMatA);
    // glm::mat3 L_invInertiaB = L_rotMatB * L_bObject->GetInverseInertiaTensor() * glm::transpose(L_rotMatB);

    // glm::vec3 L_crossA = glm::cross(L_torqueArmA, L_contact.normal);
    // glm::vec3 L_crossB = glm::cross(L_torqueArmB, L_contact.normal);
    // float L_denominator = L_totalInverseMass + 
    //                     glm::dot(L_crossA, L_invInertiaA * L_crossA) + 
    //                     glm::dot(L_crossB, L_invInertiaB * L_crossB);

    // float L_restitution = glm::min(L_aObject->GetRestitution(), L_bObject->GetRestitution());
    // float L_numerator = -(1.0f + L_restitution) * L_velAlongNormal;

    // float L_normalImpulseMag = L_numerator / L_denominator;
    // glm::vec3 L_impulse = L_contact.normal * L_normalImpulseMag;

    // float L_frictionEffectA = L_bObject->GetFriction();
    // float L_frictionEffectB = L_aObject->GetFriction();

    // // Apply impulses
    // L_aObject->SetVelocity((L_aObject->GetVelocity() - L_impulse * L_invMassA) * L_frictionEffectA);
    // L_bObject->SetVelocity((L_bObject->GetVelocity() + L_impulse * L_invMassB) * L_frictionEffectB);

    // // Angular velocity update
    // L_aObject->SetAngularVelocity(L_aObject->GetAngularVelocity() - 
    //     L_invInertiaA * glm::cross(L_torqueArmA, L_impulse));
    // L_bObject->SetAngularVelocity(L_bObject->GetAngularVelocity() + 
    //     L_invInertiaB * glm::cross(L_torqueArmB, L_impulse));

    // Friction (tangential impulse)
    

    // printf("New Pos: %f, %f %f\nNew Pos: %f, %f %f\nNew Vel: %f, %f %f\nNew Vel: %f, %f %f\n",
    //         L_aObject->GetTransform().GetPosition().x, L_aObject->GetTransform().GetPosition().y, L_aObject->GetTransform().GetPosition().z,
    //         L_bObject->GetTransform().GetPosition().x, L_bObject->GetTransform().GetPosition().y, L_bObject->GetTransform().GetPosition().z, 
    //         L_aObject->GetVelocity().x, L_aObject->GetVelocity().y, L_aObject->GetVelocity().z,
    //         L_bObject->GetVelocity().x, L_bObject->GetVelocity().y, L_bObject->GetVelocity().z);
}