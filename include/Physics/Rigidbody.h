#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "GLPipeline/Model.h"

#include "glm/ext.hpp"
#include <memory>
#include <vector>
#include <algorithm>

class Rigidbody
{
    private:
    bool m_isKinematic;
    std::weak_ptr<Transform> m_Transform;

    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;

    float m_LinearDamping;

    float m_Friction;
    public:

    Rigidbody(std::weak_ptr<Transform> _transform);
    ~Rigidbody();

    void Update(float _delta);

    void AddForce(glm::vec3 _force);

    void SetVelocity(glm::vec3 _velocity);
    void SetAcceleration(glm::vec3 _accel);

    void SetFriction(float _friction);

    void SetTransform(std::weak_ptr<Transform> _transform);

    bool GetKinematic();
    glm::vec3 GetVelocity();
    glm::vec3 GetAcceleration();

    float GetFriction();
    float GetLinearDamping();
    
    std::weak_ptr<Transform> GetTransform();
};

#endif
