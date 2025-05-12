#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "GLPipeline/Model.h"

#include "glm/ext.hpp"
#include <memory>
#include <vector>
#include <algorithm>

class Physics
{
    private:
    bool m_isKinematic;
    std::weak_ptr<Transform> m_Transform;
    std::shared_ptr<CollisionShape> m_Shape;

    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;

    float m_Mass;
    glm::vec3 m_PreviousPos;
    glm::vec3 m_Force;
    glm::vec3 m_Torque;
    glm::vec3 m_AngularVelocity;
    glm::mat3 m_InverseInertiaTensor;
    glm::mat3 m_InverseBodyInertiaTensor;

    float m_LinearDamping;
    float m_AngularDamping;

    float m_Restitution;
    float m_Friction;

    void Kinematic(float _delta);
    void Euler(float _delta);
    public:

    Physics(std::weak_ptr<Transform> _transform);
    Physics(float _radius, std::weak_ptr<Transform> _transform);
    Physics(glm::vec3 _size, std::weak_ptr<Transform> _transform);
    Physics(std::shared_ptr<Model> _model, std::shared_ptr<Transform> _transform);
    ~Physics();

    void Update(float _delta);

    void AddForce(glm::vec3 _force);
    void AddForceAtPosition(glm::vec3 _force, glm::vec3 _position);
    void AddTorque(glm::vec3 _torque);
    glm::vec3 ComputeTorque(glm::vec3 _torqueArm, glm::vec3 _contactForce);
    
    void SetFriction(float _friction);
    void SetRestitution(float _elastic);
    void SetLinearDamping(float _damp);
    void SetAngularDamping(float _damp);
    void SetMass(float _mass);
    void SetVelocity(glm::vec3 _velocity);
    void SetAngularVelocity(glm::vec3 _angVel);
    void SetAcceleration(glm::vec3 _accel);

    void SetCollisionShape(CollisionShape* _shape);
    void SetTransform(std::weak_ptr<Transform> _transform);

    bool GetKinematic();
    glm::vec3 GetVelocity();
    glm::vec3 GetAcceleration();

    glm::vec3 GetForce();
    glm::vec3 GetAngularVelocity();
    glm::mat3 GetInertiaTensor();
    glm::mat3 GetInverseInertiaTensor();
    float GetFriction();
    float GetMass();
    float GetRestitution();
    float GetLinearDamping();
    float GetAngularDamping();
    
    std::weak_ptr<CollisionShape> GetShape();
    std::weak_ptr<Transform> GetTransform();
};

#endif
