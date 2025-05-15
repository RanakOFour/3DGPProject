#include "Game/Scene.h"
#include "Game/Game.h"
#include "Game/GameEntity.h"
#include "Physics/Octree.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Rigidbody.h"

#include "SDL2/SDL.h"

#include <memory>

Scene::Scene(glm::vec3 _size, int _maxObjects) :
	m_Octree(glm::vec3(0.0f), _size, 8, _maxObjects),
	m_Collisions(),
    m_Entities(),
    m_Camera(),
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

	//m_Octree.Prune();

    // for (int i = 0; i < m_Entities.size(); ++i)
	// {
    //     Physics* L_entityPhysics = m_Entities[i]->GetPhysics();
    //     std::vector<std::weak_ptr<Physics>> L_potentialCollisions = m_Octree.Query(L_entityPhysics);

    //     Use index comparison to avoid duplicates
    //     for (int j = 0; j < L_potentialCollisions.size(); ++j)
	// 	{
	// 		Physics* L_entityPhysicsB = L_potentialCollisions[j].lock().get();

	// 		Iterator types are too long to type out
    //         auto L_objectPosition = std::find(m_Entities.begin(), m_Entities.end(), L_entityPhysics);
	// 		if (L_objectPosition == m_Entities.end()){continue;}
                
    //         size_t L_properPos = L_objectPosition - m_Entities.begin();

	// 		Prevents duplicates, don't add collisions from objects before this one
    //         if (L_properPos <= i){continue;}

    //         PhysicsSystem::CollisionInfo L_info;
    //         if (PhysicsSystem::CollisionDetection::CollisionCheck(*L_entityPhysics, *L_entityPhysicsB, &L_info))
	// 		{
	// 			tbb::concurent_vector so insertion is thread safe
	// 			m_Collisions.push_back(L_info);
    //         }
    //     }
    // }

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

	m_Camera.DrawSkybox();

	m_Camera.Update(_delta);

	// Update objects
    for(int i = 0; i < m_Entities.size(); ++i)
    {
        m_Entities[i]->Update(_delta);
        m_Octree.UpdateMovedObject(m_Entities[i]->GetCollider().lock().get());

		// Saves on looping the entire set again.
		m_Entities[i]->Draw(&m_Camera);
    }


}