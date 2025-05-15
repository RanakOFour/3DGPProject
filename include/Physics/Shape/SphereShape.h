#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Physics/Shape/CollisionShape.h"

class SphereShape : public CollisionShape
{
    private:
    float m_Radius;

    public:
    SphereShape(float _radius, bool _env) : CollisionShape(ShapeType::Sphere, _env)
    { m_Radius = _radius; };
    SphereShape(float _radius, std::weak_ptr<GameEntity> _parent, bool _env) : CollisionShape(ShapeType::Sphere, _parent, _env)
    { m_Radius = _radius; };
    ~SphereShape() {};

    float GetRadius() { return m_Radius; };
};

#endif