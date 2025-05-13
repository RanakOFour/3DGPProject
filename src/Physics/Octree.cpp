#include "Physics/Octree.h"
#include "glm/ext.hpp"
#include <tbb/parallel_for.h>

Octree::OctreeNode::OctreeNode(glm::vec3 _center, glm::vec3 _halfSize, int _depth) : 
center(_center),
halfSize(_halfSize),
depth(_depth),
objects(),
children()
{
}

bool const Octree::OctreeNode::IsLeaf()
{
    return !children[0];
}

bool const Octree::OctreeNode::Contains(Physics* _obj)
{
    glm::vec3 L_pos = _obj->GetTransform().lock()->GetPosition();
    glm::vec3 L_objHalfSize(0.0f);
    CollisionShape* L_shape = _obj->GetShape().lock().get();

    if (L_shape->GetType() == ShapeType::Sphere)
    {
        float radius = ((SphereShape*)L_shape)->GetRadius();
        L_objHalfSize = glm::vec3(radius);
    }
    else
    {
        L_objHalfSize = ((CubeShape*)L_shape)->GetHalfSize();
    }

    glm::vec3 L_objMin = L_pos - L_objHalfSize;
    glm::vec3 L_objMax = L_pos + L_objHalfSize;

    glm::vec3 L_nodeMin = center - halfSize;
    glm::vec3 L_nodeMax = center + halfSize;

    return (L_objMin.x <= L_nodeMax.x) && (L_objMax.x >= L_nodeMin.x) &&
           (L_objMin.y <= L_nodeMax.y) && (L_objMax.y >= L_nodeMin.y) &&
           (L_objMin.z <= L_nodeMax.z) && (L_objMax.z >= L_nodeMin.z);
}

void Octree::OctreeNode::CreateChildren()
{
    glm::vec3 L_childHalfSize = halfSize * 0.5f;
    for (int i = 0; i < 8; ++i)
    {
        glm::vec3 L_offset(
            (i & 1) ? L_childHalfSize.x : -L_childHalfSize.x,
            (i & 2) ? L_childHalfSize.y : -L_childHalfSize.y,
            (i & 4) ? L_childHalfSize.z : -L_childHalfSize.z);
        children[i] = std::make_unique<OctreeNode>(center + L_offset, L_childHalfSize, depth + 1);
    }
}

void Octree::OctreeNode::Insert(Physics* _obj, int _maxDepth, int _maxObjects,
                                std::unordered_map<Physics*, std::vector<OctreeNode*>>& _objectNodes)
{
    if (!Contains(_obj))
        return;

    if (IsLeaf())
    {
        objects.push_back(_obj);
        _objectNodes[_obj].push_back(this);

        if (objects.size() > _maxObjects && depth < _maxDepth)
        {
            CreateChildren();
            for (int i = 0; i < objects.size(); ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (children[j]->Contains(objects[i]))
                    {
                        children[j]->Insert(objects[i], _maxDepth, _maxObjects, _objectNodes);
                    }
                }
            }

            // Update object-node mappings
            for (int i = 0; i < objects.size(); ++i)
            {
                // I will slit my wrists before writing out a whole ass iterator type
                auto L_iteratorLocation = _objectNodes.find(objects[i]);
                if (L_iteratorLocation != _objectNodes.end())
                {
                    L_iteratorLocation->second.erase(std::remove(L_iteratorLocation->second.begin(), L_iteratorLocation->second.end(), this),
                                                     L_iteratorLocation->second.end());
                }
            }
            objects.clear();
        }
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            if (children[i]->Contains(_obj))
            {
                children[i]->Insert(_obj, _maxDepth, _maxObjects, _objectNodes);
            }
        }
    }
}

void Octree::OctreeNode::Remove(Physics* _obj,
                                std::unordered_map<Physics*, std::vector<OctreeNode*>>& _objectNodes)
{
    auto L_iteratorLocation = std::find(objects.begin(), objects.end(), _obj);
    if (L_iteratorLocation != objects.end())
    {
        objects.erase(L_iteratorLocation);
        auto L_mapIterator = _objectNodes.find(_obj);
        if (L_mapIterator != _objectNodes.end())
        {
            auto L_nodeIterator = std::find(L_mapIterator->second.begin(), L_mapIterator->second.end(), this);
            if (L_nodeIterator != L_mapIterator->second.end())
            {
                L_mapIterator->second.erase(L_nodeIterator);
            }
        }
    }

    if (!IsLeaf())
    {
        for (int i = 0; i < 8; ++i)
        {
            if (children[i])
            {
                children[i]->Remove(_obj, _objectNodes);
            }
        }
    }
}

void const Octree::OctreeNode::CollectObjects(std::vector<Physics*>& _results)
{
    if (IsLeaf())
    {
        _results.insert(_results.end(), objects.begin(), objects.end());
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            if (children[i])
            {
                children[i]->CollectObjects(_results);
            }
        }
    }
}

void Octree::OctreeNode::Prune(int _maxObjects,
                               std::unordered_map<Physics *, std::vector<OctreeNode *>>& _objectNodes)
{
    if (IsLeaf())
        return;

    int L_totalObjects = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (children[i])
        {
            children[i]->Prune(_maxObjects, _objectNodes);
            L_totalObjects += children[i]->objects.size();
        }
    }

    if (L_totalObjects <= _maxObjects)
    {
        for (int i = 0; i < 8; ++i)
        {
            if (children[i])
            {
                // Transfer objects to parent
                objects.insert(objects.end(), children[i]->objects.begin(), children[i]->objects.end());
                // Update node mappings
                for (int j = 0; j < children[i]->objects.size(); ++j)
                {
                    auto L_iteratorLocation = _objectNodes.find(children[i]->objects[j]);
                    if (L_iteratorLocation != _objectNodes.end())
                    {
                        auto L_nodeIterator = std::find(L_iteratorLocation->second.begin(), L_iteratorLocation->second.end(), children[i].get());
                        if (L_nodeIterator != L_iteratorLocation->second.end())
                        {
                            L_iteratorLocation->second.erase(L_nodeIterator);
                            L_iteratorLocation->second.push_back(this);
                        }
                    }
                }
                children[i].reset();
            }
        }
    }
}

Octree::Octree(glm::vec3 _center, glm::vec3 _size, int _maxDepth, int _maxObjects) : 
m_Root(std::make_unique<OctreeNode>(_center, _size / 2.0f, 0)),
m_MaxDepth(_maxDepth),
m_MaxObjects(_maxObjects)
{

}

Octree::~Octree()
{

};

void Octree::Insert(Physics* _obj)
{
    m_Root->Insert(_obj, m_MaxDepth, m_MaxObjects, m_ObjectNodes);
}

void Octree::Remove(Physics* _obj)
{
    m_Root->Remove(_obj, m_ObjectNodes);
    m_ObjectNodes.erase(_obj);
}

void Octree::Clear()
{
    m_Root = std::make_unique<OctreeNode>(m_Root->center, m_Root->halfSize, 0);
    m_ObjectNodes.clear();
}

std::vector<Physics*> const Octree::Query(Physics* _obj)
{
    std::vector<Physics*> L_results;

    auto L_iteratorLocation = m_ObjectNodes.find(_obj);
    if (L_iteratorLocation != m_ObjectNodes.end())
    {
        for(auto *node : L_iteratorLocation->second)
        {
            node->CollectObjects(L_results);
        }
    }

    // Remove duplicates
    std::sort(L_results.begin(), L_results.end());
    L_results.erase(std::unique(L_results.begin(), L_results.end()), L_results.end());
    // Remove self
    L_results.erase(std::remove(L_results.begin(), L_results.end(), _obj), L_results.end());
    return L_results;
}

void Octree::UpdateMovedObject(Physics* _obj)
{
    Remove(_obj);
    Insert(_obj);
}

void Octree::Prune()
{
    m_Root->Prune(m_MaxObjects, m_ObjectNodes);
}