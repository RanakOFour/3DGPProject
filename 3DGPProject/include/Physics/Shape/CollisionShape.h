#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <iostream>

enum ShapeType
{
    Cube = 1,
    Sphere = 2,
    Mesh = 4
};

class CollisionShape
{
    private:
    ShapeType m_Type;

    public:

    CollisionShape(ShapeType _type) { m_Type = _type; };
    ~CollisionShape() {};
    ShapeType GetType() { return m_Type; };
};

#endif
