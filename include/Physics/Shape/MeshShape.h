#ifndef MESHSHAPE_H
#define MESHSHAPE_H

#include "Physics/Shape/CollisionShape.h"
#include "GLPipeline/Model.h"
#include <memory>

class MeshShape : public CollisionShape
{
    private:
    std::weak_ptr<Model> m_Model;

    public:
    MeshShape(std::shared_ptr<Model> _model, bool _env) : CollisionShape(ShapeType::Mesh, _env),
    m_Model(_model)
    {}
    MeshShape(std::shared_ptr<Model> _model, std::weak_ptr<GameEntity> _parent, bool _env) : CollisionShape(ShapeType::Mesh, _parent, _env),
    m_Model(_model)
    {}

    std::vector<Face>& GetFaces() { return m_Model.lock()->GetFaces(); };
};

#endif
