
#include "Physics/PhysicsSystem.h"
#include "Physics/Transform.h"
#include "Physics/Physics.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/SphereShape.h"

#include "glm/ext.hpp"

bool PhysicsSystem::CollisionDetection::OBBCollision(CubeShape* _cubeA, Transform* _transformA, CubeShape* _cubeB, Transform* _transformB, CollisionInfo* _infoOut)
{
    // Get rotation matrices from quaternions
    glm::mat3 L_rotA = glm::mat3_cast(_transformA->GetRotation());
    glm::mat3 L_rotB = glm::mat3_cast(_transformB->GetRotation());

    // Get scaled half extents
    glm::vec3 L_halfA = _cubeA->GetHalfSize();
    glm::vec3 L_halfB = _cubeB->GetHalfSize();

    // Center positions
    glm::vec3 L_centerA = _transformA->GetPosition();
    glm::vec3 L_centerB = _transformB->GetPosition();

    // Calculate translation vector and rotation matrix
    glm::vec3 L_delta = L_centerB - L_centerA;
    glm::mat3 L_rotationMatrix = glm::transpose(L_rotA) * L_rotB;
    glm::mat3 L_absRotMat = glm::abs(L_rotationMatrix);

    // Transform L_delta to L_aObject's local space
    glm::vec3 L_deltaAsLocalA = glm::vec3(
        glm::dot(L_delta, L_rotA[0]),
        glm::dot(L_delta, L_rotA[1]),
        glm::dot(L_delta, L_rotA[2]));

    float L_radiusA, L_radiusB;
    float L_penetration = FLT_MAX;
    glm::vec3 L_normal;
    bool L_collisionFound = true;

    // Test axes from OBB L_aObject
    for (int i = 0; i < 3; ++i)
    {
        L_radiusA = L_halfA[i];
        L_radiusB = L_halfB.x * L_absRotMat[i][0] + L_halfB.y * L_absRotMat[i][1] + L_halfB.z * L_absRotMat[i][2];

        if (glm::abs(L_deltaAsLocalA[i]) > L_radiusA + L_radiusB)
        {
            L_collisionFound = false;
            break;
        }

        float L_testPen = L_radiusA + L_radiusB - glm::abs(L_deltaAsLocalA[i]);
        if (L_testPen < L_penetration)
        {
            L_penetration = L_testPen;
            L_normal = L_rotA[i] * glm::sign(L_deltaAsLocalA[i]);
        }
    }
    if (!L_collisionFound)
        return false;

    // Test axes from OBB B
    for (int i = 0; i < 3; ++i)
    {
        L_radiusA = L_halfA.x * L_absRotMat[0][i] + L_halfA.y * L_absRotMat[1][i] + L_halfA.z * L_absRotMat[2][i];
        L_radiusB = L_halfB[i];

        float L_projection = L_deltaAsLocalA.x * L_rotationMatrix[0][i] + L_deltaAsLocalA.y * L_rotationMatrix[1][i] + L_deltaAsLocalA.z * L_rotationMatrix[2][i];
        if (glm::abs(L_projection) > L_radiusA + L_radiusB)
        {
            L_collisionFound = false;
            break;
        }

        float L_testPen = L_radiusA + L_radiusB - glm::abs(L_projection);
        if (L_testPen < L_penetration)
        {
            L_penetration = L_testPen;
            L_normal = L_rotB[i] * glm::sign(L_projection);
            L_normal = L_rotA * L_normal; // Convert to world space
        }
    }
    if (!L_collisionFound)
        return false;

    // 3. Test edge cross products
    const float epsilon = 1e-6f;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            glm::vec3 L_axis = glm::cross(L_rotA[i], L_rotB[j]);
            float L_squareAxisLength = glm::length(L_axis);
            L_squareAxisLength *= L_squareAxisLength;
            if (L_squareAxisLength < epsilon)
                continue;

            L_axis /= glm::sqrt(L_squareAxisLength); // Normalize

            // Calculate projections
            L_radiusA = L_halfA.x * glm::abs(glm::dot(L_rotA[0], L_axis)) +
                 L_halfA.y * glm::abs(glm::dot(L_rotA[1], L_axis)) +
                 L_halfA.z * glm::abs(glm::dot(L_rotA[2], L_axis));

            L_radiusB = L_halfB.x * glm::abs(glm::dot(L_rotB[0], L_axis)) +
                 L_halfB.y * glm::abs(glm::dot(L_rotB[1], L_axis)) +
                 L_halfB.z * glm::abs(glm::dot(L_rotB[2], L_axis));

            float L_separation = glm::abs(glm::dot(L_delta, L_axis));
            if (L_separation > L_radiusA + L_radiusB)
            {
                L_collisionFound = false;
                break;
            }

            float L_testPen = L_radiusA + L_radiusB - L_separation;
            if (L_testPen < L_penetration)
            {
                L_penetration = L_testPen;
                L_normal = L_axis * glm::sign(glm::dot(L_delta, L_axis));
            }
        }

        if (!L_collisionFound)
        {
            break;
        }
    }
    
    if (!L_collisionFound)
    {
        return false;
    }

    // Ensure L_normal points from B to A
    if (glm::dot(L_normal, L_delta) < 0.0f)
    {
        L_normal = -L_normal;
    }

    glm::vec3 L_worldContactA = L_centerA + L_normal * (L_penetration * 0.5f);
    glm::vec3 L_worldContactB = L_centerB - L_normal * (L_penetration * 0.5f);

    // Convert to local space using rotation transpose
    glm::vec3 L_localA = glm::transpose(L_rotA) * (L_worldContactA - L_centerA);
    glm::vec3 L_localB = glm::transpose(L_rotB) * (L_worldContactB - L_centerB);

    _infoOut->AddContactPoint(
        L_localA,
        L_localB,
        L_normal,
        L_penetration
    );
    return true;
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

bool PhysicsSystem::CollisionDetection::CollisionCheck(Physics &_aObject, Physics &_bObject, CollisionInfo *_infoOut)
{
    std::weak_ptr<CollisionShape> L_shapeA = _aObject.GetShape();
    std::weak_ptr<CollisionShape> L_shapeB = _bObject.GetShape();

    _infoOut->objectA = &_aObject;
    _infoOut->objectB = &_bObject;

    std::weak_ptr<Transform> L_transformA = _aObject.GetTransform();
    std::weak_ptr<Transform> L_transformB = _bObject.GetTransform();

    int L_shapeTypeA = (int)L_shapeA.lock()->GetType();
    int L_shapeTypeB = (int)L_shapeB.lock()->GetType();

    ShapeType L_shapeType = (ShapeType)(((int)L_shapeA.lock()->GetType()) | ((int)L_shapeB.lock()->GetType()));

    if (L_shapeType == ShapeType::Cube)
    {
        return OBBCollision((CubeShape*)L_shapeA.lock().get(), L_transformA.lock().get(), (CubeShape*)L_shapeB.lock().get(), L_transformB.lock().get(), _infoOut);
    }
    else if (L_shapeType == ShapeType::Sphere)
    {
        return SphereCollision((SphereShape*)L_shapeA.lock().get(), L_transformA.lock().get(), (SphereShape*)L_shapeB.lock().get(), L_transformB.lock().get(), _infoOut);
    }
    else if (L_shapeA.lock()->GetType() == ShapeType::Cube)
    {
        return OBBvsSphereCollision((CubeShape*)L_shapeA.lock().get(), L_transformA.lock().get(), (SphereShape*)L_shapeB.lock().get(), L_transformB.lock().get(), _infoOut);
    }
    else
    {
        _infoOut->objectA = &_bObject;
        _infoOut->objectB = &_aObject;
        return OBBvsSphereCollision((CubeShape*)L_shapeB.lock().get(), L_transformB.lock().get(), (SphereShape*)L_shapeA.lock().get(), L_transformA.lock().get(), _infoOut);
    }
}

void PhysicsSystem::CollisionDetection::ImpulseCollisionResolution(CollisionInfo* _info)
{
    Physics* L_aObject = _info->objectA;
    Physics* L_bObject = _info->objectB;
    ContactPoint L_contact = _info->contactPoint;

    // Inverse masses
    float L_invMassA = (L_aObject->GetMass() == INFINITY) ? 0.0f : 1.0f / L_aObject->GetMass();
    float L_invMassB = (L_bObject->GetMass() == INFINITY) ? 0.0f : 1.0f / L_bObject->GetMass();
    float L_totalInverseMass = L_invMassA + L_invMassB;
    if(L_totalInverseMass <= 0) return;

    glm::vec3 L_correction = L_contact.normal * (L_contact.penetration / L_totalInverseMass);
    L_aObject->GetTransform().lock()->Move(-L_correction * L_invMassA);
    L_bObject->GetTransform().lock()->Move(L_correction * L_invMassB);

    // Calculate torque arms in world space
    glm::quat L_rotA = L_aObject->GetTransform().lock()->GetRotation();
    glm::quat L_rotB = L_bObject->GetTransform().lock()->GetRotation();
    glm::vec3 L_torqueArmA = L_rotA * L_contact.localA;
    glm::vec3 L_torqueArmB = L_rotB * L_contact.localB;

    // Relative velocity
    glm::vec3 L_velA = L_aObject->GetVelocity() + glm::cross(L_aObject->GetAngularVelocity(), L_torqueArmA);
    glm::vec3 L_velB = L_bObject->GetVelocity() + glm::cross(L_bObject->GetAngularVelocity(), L_torqueArmB);
    glm::vec3 L_relVel = L_velB - L_velA;

    // Normal impulse
    float L_velAlongNormal = glm::dot(L_relVel, L_contact.normal);
    if (L_velAlongNormal > 0) return;

    // Rotate inertia tensors to world space
    glm::mat3 L_rotMatA = glm::mat3_cast(L_rotA);
    glm::mat3 L_rotMatB = glm::mat3_cast(L_rotB);
    glm::mat3 L_invInertiaA = L_rotMatA * L_aObject->GetInverseInertiaTensor() * glm::transpose(L_rotMatA);
    glm::mat3 L_invInertiaB = L_rotMatB * L_bObject->GetInverseInertiaTensor() * glm::transpose(L_rotMatB);

    glm::vec3 L_crossA = glm::cross(L_torqueArmA, L_contact.normal);
    glm::vec3 L_crossB = glm::cross(L_torqueArmB, L_contact.normal);
    float L_denominator = L_totalInverseMass + 
                        glm::dot(L_crossA, L_invInertiaA * L_crossA) + 
                        glm::dot(L_crossB, L_invInertiaB * L_crossB);

    float L_restitution = glm::min(L_aObject->GetRestitution(), L_bObject->GetRestitution());
    float L_numerator = -(1.0f + L_restitution) * L_velAlongNormal;

    float L_normalImpulseMag = L_numerator / L_denominator;
    glm::vec3 L_impulse = L_contact.normal * L_normalImpulseMag;

    float L_frictionEffectA = L_bObject->GetFriction();
    float L_frictionEffectB = L_aObject->GetFriction();

    // Apply impulses
    L_aObject->SetVelocity((L_aObject->GetVelocity() - L_impulse * L_invMassA) * L_frictionEffectA);
    L_bObject->SetVelocity((L_bObject->GetVelocity() + L_impulse * L_invMassB) * L_frictionEffectB);

    // Angular velocity update
    L_aObject->SetAngularVelocity(L_aObject->GetAngularVelocity() - 
        L_invInertiaA * glm::cross(L_torqueArmA, L_impulse));
    L_bObject->SetAngularVelocity(L_bObject->GetAngularVelocity() + 
        L_invInertiaB * glm::cross(L_torqueArmB, L_impulse));

    // Friction (tangential impulse)
    

    // printf("New Pos: %f, %f %f\nNew Pos: %f, %f %f\nNew Vel: %f, %f %f\nNew Vel: %f, %f %f\n",
    //         L_aObject->GetTransform().GetPosition().x, L_aObject->GetTransform().GetPosition().y, L_aObject->GetTransform().GetPosition().z,
    //         L_bObject->GetTransform().GetPosition().x, L_bObject->GetTransform().GetPosition().y, L_bObject->GetTransform().GetPosition().z, 
    //         L_aObject->GetVelocity().x, L_aObject->GetVelocity().y, L_aObject->GetVelocity().z,
    //         L_bObject->GetVelocity().x, L_bObject->GetVelocity().y, L_bObject->GetVelocity().z);
}