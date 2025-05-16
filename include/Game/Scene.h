#ifndef SCENE_H
#define SCENE_H

#include "Game/Camera.h"
#include "Game/GameEntity.h"
#include "Physics/PhysicsSystem.h"

#include "SDL2/SDL.h"

#include <vector>
#include <memory>

class Game;
class HUD;
class Scene
{
private:
	std::weak_ptr<Game> m_Game;

	// Thread safe vector
	std::vector<PhysicsSystem::CollisionInfo> m_Collisions;

	std::shared_ptr<GameEntity> m_Player;
	std::vector<std::shared_ptr<GameEntity>> m_Entities;

	std::unique_ptr<HUD> m_hud;

	Camera m_Camera;
	float m_CurrentDelta;
	float m_TimeElapsed;

	bool m_ToggleSkybox;
	bool m_ToggleGUI;
public:
	Scene();
	~Scene();

	void SetGame(std::weak_ptr<Game> _game);
	void SetPlayer(std::shared_ptr<GameEntity> _player);
	void AddEntity(std::shared_ptr<GameEntity>& _entity);
	void AddElement(const std::string &_texturePath, glm::vec2 _position, glm::vec2 _size);
	void Update(float _delta, const uint8_t* _keyInputs);

	std::weak_ptr<Game> GetGame() { return m_Game; };

	// Going to assume I won't call this in a bad way
	std::weak_ptr<GameEntity> GetEntity(int _id) { return m_Entities[_id]; };

	std::shared_ptr<GameEntity> GetPlayer() { return m_Entities[0]; };
	HUD* GetHUD() { return m_hud.get(); };
	float GetDelta() { return m_CurrentDelta; };
	float GetTime() { return m_TimeElapsed; };
};

#endif
