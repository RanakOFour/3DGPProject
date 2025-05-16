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
    std::weak_ptr<Transform> m_Transform;

    float m_Mass;
    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;
    glm::vec3 m_Force;

    public:

    Rigidbody(std::weak_ptr<Transform> _transform, float m_Mass);
    ~Rigidbody();

    void Update(float _delta);

    void AddForce(glm::vec3 _force);

    void SetVelocity(glm::vec3 _velocity);
    void SetAcceleration(glm::vec3 _accel);

    void SetTransform(std::weak_ptr<Transform> _transform);

    float GetMass() { return m_Mass; };
    glm::vec3 GetVelocity();
    glm::vec3 GetAcceleration();
    
    std::weak_ptr<Transform> GetTransform();
};

#endif
