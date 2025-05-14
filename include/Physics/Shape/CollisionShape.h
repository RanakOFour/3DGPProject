#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include "Physics/Transform.h"

#include <iostream>
#include <memory>

enum ShapeType
{
    Cube = 1,
    Sphere = 2,
    Mesh = 4
};

class CollisionShape
{
    private:
    std::weak_ptr<Transform> m_ParentTransform;
    ShapeType m_Type;
    bool m_Environment;

    public:

    CollisionShape(ShapeType _type, std::weak_ptr<Transform> _trans, bool _env)
    {
        m_Type = _type;
        m_ParentTransform = _trans;
        m_Environment = _env;
    };

    ~CollisionShape() {};
    ShapeType GetType() { return m_Type; };
    Transform* GetTransform() { return m_ParentTransform.lock().get(); };
    bool Environment() { return m_Environment; };
};

#endif
