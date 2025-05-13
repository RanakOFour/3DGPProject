
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

// Helper function to project a triangle onto an axis
inline void ProjectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, 
                    const glm::vec3& axis, float& min, float& max)
{
    const float d1 = glm::dot(a, axis);
    const float d2 = glm::dot(b, axis);
    const float d3 = glm::dot(c, axis);
    
    min = std::min(std::min(d1, d2), d3);
    max = std::max(std::max(d1, d2), d3);
}

// SAT test between two triangles
inline bool TestTrianglesSAT(const Face& tri1, const Face& tri2, PhysicsSystem::ContactPoint& contact)
{
    const glm::vec3& a1 = tri1.a.position;
    const glm::vec3& b1 = tri1.b.position;
    const glm::vec3& c1 = tri1.c.position;
    
    const glm::vec3& a2 = tri2.a.position;
    const glm::vec3& b2 = tri2.b.position;
    const glm::vec3& c2 = tri2.c.position;
    
    // Potential separating axes
    glm::vec3 axes[11] = {
        tri1.normal,
        tri2.normal,
        glm::cross(b1 - a1, b2 - a2),
        glm::cross(b1 - a1, c2 - a2),
        glm::cross(b1 - a1, c2 - b2),
        glm::cross(c1 - a1, b2 - a2),
        glm::cross(c1 - a1, c2 - a2),
        glm::cross(c1 - a1, c2 - b2),
        glm::cross(c1 - b1, b2 - a2),
        glm::cross(c1 - b1, c2 - a2),
        glm::cross(c1 - b1, c2 - b2)
    };
    
    float minDepth = FLT_MAX;
    glm::vec3 minNormal;
    
    for (int i = 0; i < 11; ++i)
    {
        if (glm::length(axes[i]) < 0.001f) continue; // Skip near-zero axes
        
        glm::vec3 normAxis = glm::normalize(axes[i]);
        
        float min1, max1, min2, max2;
        ProjectTriangle(a1, b1, c1, normAxis, min1, max1);
        ProjectTriangle(a2, b2, c2, normAxis, min2, max2);
        
        if (!(max1 >= min2 & max2 >= min1))
        {
            return false; // Separating axis found
        }
        

        // Calculate penetration depth for this axis
        float overlap = std::min(max1, max2) - std::max(min1, min2);
        if (overlap < minDepth)
        {
            minDepth = overlap;
            minNormal = normAxis;
        }
    }
    
    // If we get here, all axes showed overlap - collision exists
    // Determine the contact point
    
    glm::vec3 vertices[6] = {
        tri1.a.position,
        tri1.b.position,
        tri1.c.position,
        tri2.a.position,
        tri2.b.position,
        tri2.c.position
    };
    
    // Find the vertex most in the direction of the normal (from B to A)
    float maxDot = -FLT_MAX;
    glm::vec3 maxVertex;
    
    for (int i = 0; i < 6; ++i)
    {
        float dot = glm::dot(vertices[i], minNormal);
        if (dot > maxDot)
        {
            maxDot = dot;
            maxVertex = vertices[i];
        }
    }
    
    contact.localA = maxVertex;
    contact.localB = maxVertex;
    contact.normal = minNormal;
    contact.penetration = minDepth;
    
    return true;
}

/*
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
*/

bool PhysicsSystem::CollisionDetection::MeshCollision(MeshShape* _meshA, Transform* _transformA, MeshShape* _meshB, Transform* _transformB, CollisionInfo* _infoOut)
{
    // Assuming Physics class has a GetModel() method that returns a Model*
    std::vector<Face>& L_facesA = _meshA->GetFaces();
    std::vector<Face>& L_facesB = _meshB->GetFaces();
    
    ContactPoint L_bestContact;
    L_bestContact.penetration = -1.0f; // Initialize to invalid value
    
    for (int i = 0; i < L_facesA.size(); ++i) 
    {
        for (int j = 0; j < L_facesB.size(); ++j) 
        {
            ContactPoint L_currentContact;
            if (TestTrianglesSAT(L_facesA[i], L_facesB[j], L_currentContact) && 
                L_currentContact.penetration > L_bestContact.penetration)
            {
                // Keep the contact with deepest penetration
                L_bestContact = L_currentContact;
            }
        }
    }
    
    if (L_bestContact.penetration >= 0.0f)
    {
        // Collision found - create and return CollisionInfo
        _infoOut->AddContactPoint(L_bestContact.localA, L_bestContact.localB, 
                            L_bestContact.normal, L_bestContact.penetration);
        printf("Colliding!\n");
        return true;
    }

    printf("Not Colliding!\n");
    return false; // No collision
}

bool PhysicsSystem::CollisionDetection::CollisionCheck(Physics &_aObject, Physics &_bObject, CollisionInfo *_infoOut)
{
    CollisionShape* L_shapeA = _aObject.GetShape().lock().get();
    CollisionShape* L_shapeB = _bObject.GetShape().lock().get();

    _infoOut->objectA = &_aObject;
    _infoOut->objectB = &_bObject;

    Transform* L_transformA = _aObject.GetTransform().lock().get();
    Transform* L_transformB = _bObject.GetTransform().lock().get();

    int L_shapeTypeA = (int)L_shapeA->GetType();
    int L_shapeTypeB = (int)L_shapeB->GetType();

    ShapeType L_shapeType = (ShapeType)(((int)L_shapeA->GetType()) | ((int)L_shapeB->GetType()));

    if (L_shapeType == ShapeType::Cube)
    {
        return OBBCollision((CubeShape*)L_shapeA, L_transformA, (CubeShape*)L_shapeB, L_transformB, _infoOut);
    }
    else if (L_shapeType == ShapeType::Sphere)
    {
        return SphereCollision((SphereShape*)L_shapeA, L_transformA, (SphereShape*)L_shapeB, L_transformB, _infoOut);
    }
    else if(L_shapeType == ShapeType::Mesh)
    {
        return MeshCollision((MeshShape*)L_shapeA, L_transformA, (MeshShape*)L_shapeB, L_transformB, _infoOut);
    }
    
    if (L_shapeA->GetType() == ShapeType::Cube)
    {
        if(L_shapeB->GetType() == ShapeType::Sphere)
        {
            return OBBvsSphereCollision((CubeShape*)L_shapeA, L_transformA, (SphereShape*)L_shapeB, L_transformB, _infoOut);
        }
        else
        { // Mesh

        }
    }
    if (L_shapeA->GetType() == ShapeType::Sphere)
    {
        if(L_shapeB->GetType() == ShapeType::Cube)
        {
            _infoOut->objectA = &_bObject;
            _infoOut->objectB = &_aObject;
            return OBBvsSphereCollision((CubeShape*)L_shapeB, L_transformB, (SphereShape*)L_shapeA, L_transformA, _infoOut);
        }
        else
        { // Mesh

        }
    }
    if (L_shapeA->GetType() == ShapeType::Mesh)
    {
        if(L_shapeB->GetType() == ShapeType::Cube)
        {
            
        }
        else
        { //Sphere

        }
    }

    return false;
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