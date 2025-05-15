#ifndef SCENE_H
#define SCENE_H

#include "Game/Camera.h"
#include "Game/GameEntity.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Octree.h"

#include "SDL2/SDL.h"

#include <vector>
#include <memory>

class Game;
class Scene
{
private:
	std::weak_ptr<Game> m_Game;
	// Spacial partitioning method
	Octree m_Octree;

	// Thread safe vector
	std::vector<PhysicsSystem::CollisionInfo> m_Collisions;

	std::shared_ptr<GameEntity> m_Player;
	std::vector<std::shared_ptr<GameEntity>> m_Entities;

	Camera m_Camera;
	float m_CurrentDelta;
	float m_TimeElapsed;
public:
	Scene(glm::vec3 _size, int _maxObjects);
	~Scene();

	void SetGame(std::weak_ptr<Game> _game);
	void SetPlayer(std::shared_ptr<GameEntity> _player);
	void AddEntity(std::shared_ptr<GameEntity>& _entity);
	void Update(float _delta, const uint8_t* _keyInputs);

	std::weak_ptr<Game> GetGame() { return m_Game; };
	std::shared_ptr<GameEntity> GetPlayer() { return m_Entities[0]; };
	float GetDelta() { return m_CurrentDelta; };
	float GetTime() { return m_TimeElapsed; };
};

#endif
