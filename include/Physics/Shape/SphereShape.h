#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Physics/Shape/CollisionShape.h"

class SphereShape : public CollisionShape
{
    private:
    float m_Radius;

    public:
    SphereShape(float _radius, std::weak_ptr<Transform> _trans, bool _env) : CollisionShape(ShapeType::Sphere, _trans, _env)
    { m_Radius = _radius; };
    ~SphereShape() {};

    float GetRadius() { return m_Radius; };
};

#endif