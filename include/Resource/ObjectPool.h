#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <unordered_map>
#include <memory>

/**
 * Generic definition of an object pool, used to load textures and models early before
 * throwing them into OpenGL or attaching them to entities, this prevents wasting time
 * reloading then 
 * */

template <class T> class ObjectPool
{
    private:
    // Maps file path to pointer of loaded data in memory
    std::unordered_map<std::string, std::shared_ptr<T>> m_Objects;

    public:
    ObjectPool();
    ~ObjectPool();

    void Add(std::string& _key, T* _object);
    void Remove(std::string& _key);

    std::weak_ptr<T> Find(std::string& _key);
};

#endif
