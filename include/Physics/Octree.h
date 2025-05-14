#ifndef OCTREE_H
#define OCTREE_H

#include "Physics/Shape/CollisionShape.h"
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
        std::vector<CollisionShape*> objects;
        std::unique_ptr<OctreeNode> children[8];

        OctreeNode(glm::vec3 _center, glm::vec3 _halfSize, int _depth);

        bool const IsLeaf();

        bool const Contains(CollisionShape* _obj);
        void CreateChildren();
        void Insert(CollisionShape* _obj, int _maxDepth, int _maxObjects,
                    std::unordered_map<CollisionShape*, std::vector<OctreeNode*>>& _objectNodes);
        void Remove(CollisionShape* _obj,
                    std::unordered_map<CollisionShape*, std::vector<OctreeNode*>>& _objectNodes);
        void const CollectObjects(std::vector<CollisionShape*>& _results);
        void Prune(int _maxObjects, 
                   std::unordered_map<CollisionShape*, std::vector<OctreeNode*>>& _objectNodes);
    };

    std::unique_ptr<OctreeNode> m_Root;
    int m_MaxDepth;
    int m_MaxObjects;

    // Implementing this as smart pointers looks disgusting
    std::unordered_map<CollisionShape*, std::vector<OctreeNode*>> m_ObjectNodes;

public:
    Octree(glm::vec3 center, glm::vec3 size, int maxDepth, int maxObjects);
    ~Octree();

    void Insert(CollisionShape* _object);
    void Remove(CollisionShape* _object);
    void Clear();
    std::vector<CollisionShape*> const Query(CollisionShape* _object);
    void UpdateMovedObject(CollisionShape* _object);
    void Prune();
};

#endif
