
#include "Physics/PhysicsSystem.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"

#include "Physics/Rigidbody.h"

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

    // Unfinished
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
    
    if (_aShape->GetType() == ShapeType::Cube)
    {
        _infoOut->objectA = _bShape;
        _infoOut->objectB = _aShape;
        return MeshCubeCollision((MeshShape*)_bShape, L_transformB, (CubeShape*)_aShape, L_transformA, _infoOut);
    }
    
    if (_aShape->GetType() == ShapeType::Mesh)
    {
        return MeshCubeCollision((MeshShape*)_aShape, L_transformA, (CubeShape*)_bShape, L_transformB, _infoOut);
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

    L_aObject->GetRigidbody()->GetTransform().lock()->Move(-L_correction / L_aObject->GetRigidbody()->GetMass());
    L_bObject->GetRigidbody()->GetTransform().lock()->Move(L_correction / L_bObject->GetRigidbody()->GetMass());

    glm::vec3 L_relativeVel = L_bObject->GetRigidbody()->GetVelocity() - L_aObject->GetRigidbody()->GetVelocity();

    float L_velAlongNormal = glm::dot(L_contact.normal, L_relativeVel);
    if(L_velAlongNormal > 0.0f){ return; }

    glm::vec3 L_impulseResponse = L_velAlongNormal * L_contact.normal;

    L_aObject->GetRigidbody()->SetVelocity(
        L_aObject->GetRigidbody()->GetVelocity() + L_impulseResponse / L_aObject->GetRigidbody()->GetMass()
    );

    L_bObject->GetRigidbody()->SetVelocity(
        L_bObject->GetRigidbody()->GetVelocity() - L_impulseResponse / L_bObject->GetRigidbody()->GetMass()
    );

    // Silly physics things I don't need
    // Newton died a virgin
    // // Relative velocity
    // glm::vec3 L_velA = L_aObject->GetVelocity() + glm::cross(L_aObject->GetAngularVelocity(), L_torqueArmA);
    // glm::vec3 L_velB = L_bObject->GetVelocity() + glm::cross(L_bObject->GetAngularVelocity(), L_torqueArmB);
    // glm::vec3 L_relVel = L_velB - L_velA;

    // // Normal impulse
    // float L_velAlongNormal = glm::dot(L_relVel, L_contact.normal);
    // if (L_velAlongNormal > 0) return;

    // glm::vec3 L_crossA = glm::cross(L_torqueArmA, L_contact.normal);
    // glm::vec3 L_crossB = glm::cross(L_torqueArmB, L_contact.normal);
    // float L_denominator = L_totalInverseMass + 
    //                     glm::dot(L_crossA, L_invInertiaA * L_crossA) + 
    //                     glm::dot(L_crossB, L_invInertiaB * L_crossB);

    // float L_restitution = glm::min(L_aObject->GetRestitution(), L_bObject->GetRestitution());
    // float L_numerator = -(1.0f + L_restitution) * L_velAlongNormal;

    // float L_normalImpulseMag = L_numerator / L_denominator;
    // glm::vec3 L_impulse = L_contact.normal * L_normalImpulseMag;
}