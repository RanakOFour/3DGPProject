#ifndef OCTREE_H
#define OCTREE_H

#include "Physics/Physics.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/SphereShape.h"
#include <glm/ext.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

class Octree
{
private:
    struct OctreeNode
    {
        glm::vec3 center;
        glm::vec3 halfSize;
        int depth;
        std::vector<Physics*> objects;
        std::unique_ptr<OctreeNode> children[8];

        OctreeNode(glm::vec3 _center, glm::vec3 _halfSize, int _depth);

        bool const IsLeaf();

        bool const Contains(Physics* _obj);
        void CreateChildren();
        void Insert(Physics* _obj, int _maxDepth, int _maxObjects,
                    std::unordered_map<Physics*, std::vector<OctreeNode*>>& _objectNodes);
        void Remove(Physics* _obj,
                    std::unordered_map<Physics*, std::vector<OctreeNode*>>& _objectNodes);
        void const CollectObjects(std::vector<Physics*>& _results);
        void Prune(int _maxObjects, 
                   std::unordered_map<Physics*, std::vector<OctreeNode*>>& _objectNodes);
    };

    std::unique_ptr<OctreeNode> m_Root;
    int m_MaxDepth;
    int m_MaxObjects;

    // Implementing this as smart pointers looks disgusting
    std::unordered_map<Physics*, std::vector<OctreeNode*>> m_ObjectNodes;

public:
    Octree(glm::vec3 center, glm::vec3 size, int maxDepth, int maxObjects);
    ~Octree();

    void Insert(Physics* _object);
    void Remove(Physics* _object);
    void Clear();
    std::vector<Physics*> const Query(Physics* _object);
    void UpdateMovedObject(Physics* _object);
    void Prune();
};

#endif
