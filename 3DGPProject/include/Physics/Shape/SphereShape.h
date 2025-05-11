#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Physics/Shape/CollisionShape.h"

class SphereShape : public CollisionShape
{
    private:
    float m_Radius;

    public:
    SphereShape(float _radius) : CollisionShape(ShapeType::Sphere)
    { m_Radius = _radius; };
    ~SphereShape() {};

    float GetRadius() { return m_Radius; };
};

#endif