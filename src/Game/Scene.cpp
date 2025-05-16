#include "Game/Scene.h"
#include "Game/Game.h"
#include "Game/GameEntity.h"
#include "Physics/PhysicsSystem.h"

#include "SDL2/SDL.h"

#include <memory>

Scene::Scene() :
	m_Collisions(),
    m_Entities(),
    m_Camera(true),
	m_Player(),
	m_CurrentDelta(0.0f),
	m_TimeElapsed(0.0f)
{
}

Scene::~Scene()
{

}

void Scene::SetGame(std::weak_ptr<Game> _game)
{
	m_Game = _game;
	m_Camera.SetScene(m_Game.lock()->GetScene());
}

void Scene::SetPlayer(std::shared_ptr<GameEntity> _player)
{
	m_Player = _player;
}

void Scene::AddEntity(std::shared_ptr<GameEntity>& _entity)
{
	m_Entities.push_back(_entity);
	_entity->SetID(m_Entities.size() - 1);
	_entity->SetScene(m_Game.lock()->GetScene());
	m_Camera.SetTarget(m_Entities[0]);
}

void Scene::Update(float _delta, const uint8_t* _inputKeys)
{
	m_CurrentDelta = _delta;
	m_TimeElapsed += _delta;
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 25.0f) * _delta;
	glm::vec3 leftMove = glm::vec3(25.0f, 0.0f, 0.0f) * _delta;
	glm::vec3 rotateY = glm::vec3(0.0f, 25.0f, 0.0f) * _delta;

	if(_inputKeys[SDL_SCANCODE_A])
	{
		m_Player->Move(leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_D])
	{
		m_Player->Move(-leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_W])
	{
		m_Player->Move(forward);
	}

	if(_inputKeys[SDL_SCANCODE_S])
	{
		m_Player->Move(-forward);
	}

	for(int i = 0; i < m_Entities.size(); ++i)
	{
		std::weak_ptr<CollisionShape> L_shapeA = m_Entities[i]->GetCollider();
		for(int j = i + 1; j < m_Entities.size(); ++j)
		{
			std::weak_ptr<CollisionShape> L_shapeB = m_Entities[j]->GetCollider();

			printf("Collision Check %d %d\n", m_Entities[i]->GetID(), m_Entities[j]->GetID());
			PhysicsSystem::CollisionInfo L_info;
			if(PhysicsSystem::CollisionDetection::CollisionCheck(L_shapeA.lock().get(), L_shapeB.lock().get(), &L_info))
			{
				L_shapeA.lock()->AddContact(L_shapeB);
				L_shapeB.lock()->AddContact(L_shapeA);
				m_Collisions.push_back(L_info);
			}
		}
	}

    // Resolve collisions
    for (int i = 0; i < m_Collisions.size(); ++i)
	{
        PhysicsSystem::CollisionDetection::ImpulseCollisionResolution(&m_Collisions[i]);
    }

    m_Collisions.clear();

	//m_Camera.DrawSkybox();

	m_Camera.Update(_delta);

	// Update objects
    for(int i = 0; i < m_Entities.size(); ++i)
    {
        m_Entities[i]->Update(_delta);

		// Saves on looping the entire set again.
		m_Entities[i]->Draw(&m_Camera);
    }
}