#ifndef SCENE_H
#define SCENE_H

#include "Game/Camera.h"
#include "Game/GameEntity.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Octree.h"

#include <vector>
#include <memory>

class Scene
{
private:

	// Spacial partitioning method
	//Octree m_Octree;

	// Thread safe vector
	std::vector<PhysicsSystem::CollisionInfo> m_Collisions;

	std::vector<std::shared_ptr<GameEntity>> m_Entities;

	Camera m_Camera;
public:
	Scene(glm::vec3 _size, int _maxObjects);
	~Scene();

	void AddEntity(std::shared_ptr<GameEntity>& _entity);
	void Update(float _delta, const uint8_t* _keyInputs);
};

#endif
