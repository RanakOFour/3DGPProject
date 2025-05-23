#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "Physics/Shape/CollisionShape.h"
#include "glm/ext.hpp"

class CubeShape : public CollisionShape
{
    private:
    glm::vec3 m_halfSize;

    public:
    CubeShape(glm::vec3 _halfSize, bool _env) : CollisionShape(ShapeType::Cube, _env)
    { m_halfSize = _halfSize; };
    CubeShape(glm::vec3 _halfSize, std::weak_ptr<GameEntity> _parent, bool _env) : CollisionShape(ShapeType::Cube, _parent, _env)
    { m_halfSize = _halfSize; };
    ~CubeShape() {};

    glm::vec3 GetHalfSize() { return m_halfSize; };
};

#endif
