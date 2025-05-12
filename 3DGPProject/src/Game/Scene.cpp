#include "Game/Scene.h"
#include "Game/Game.h"
#include "Game/GameEntity.h"
#include "Physics/Octree.h"
#include "Physics/PhysicsSystem.h"

#include "SDL2/SDL.h"

#include <memory>

Scene::Scene(glm::vec3 _size, int _maxObjects) :
	//m_Octree(glm::vec3(0.0f), _size, 8, _maxObjects),
	m_Collisions(),
    m_Entities(),
    m_Camera()
{

}

Scene::~Scene()
{

}

void Scene::AddEntity(std::shared_ptr<GameEntity>& _entity)
{
	m_Entities.push_back(_entity);
}

void Scene::Update(float _delta, const uint8_t* _inputKeys)
{
	HandleInputs(_inputKeys);

	printf("Camera Pos: %f, %f, %f\n", m_Camera.GetPosition().x, m_Camera.GetPosition.y, m_Camera.GetPosition.z);
	// Update objects
    for(int i = 0; i < m_Entities.size(); ++i)
    {
        m_Entities[i]->Update(_delta);
		//Physics* L_ph = m_Entities[i]->GetPhysics();
        //m_Octree.UpdateMovedObject(L_ph);
		m_Entities[i]->Draw(&m_Camera);
    }



    //m_Octree.Prune();

    // for (int i = 0; i < m_Entities.size(); ++i)
	// {
    //     Physics* L_entityPhysics = m_Entities[i]->GetPhysics();
    //     std::vector<std::weak_ptr<Physics>> L_potentialCollisions = m_Octree.Query(L_entityPhysics);

    //     // Use index comparison to avoid duplicates
    //     for (int j = 0; j < L_potentialCollisions.size(); ++j)
	// 	{
	// 		Physics* L_entityPhysicsB = L_potentialCollisions[j].lock().get();

	// 		// Iterator types are too long to type out
    //         auto L_objectPosition = std::find(m_Entities.begin(), m_Entities.end(), L_entityPhysics);
	// 		if (L_objectPosition == m_Entities.end()){continue;}
                
    //         size_t L_properPos = L_objectPosition - m_Entities.begin();

	// 		// Prevents duplicates, don't add collisions from objects before this one
    //         if (L_properPos <= i){continue;}

    //         PhysicsSystem::CollisionInfo L_info;
    //         if (PhysicsSystem::CollisionDetection::CollisionCheck(*L_entityPhysics, *L_entityPhysicsB, &L_info))
	// 		{
	// 			// tbb::concurent_vector so insertion is thread safe
	// 			m_Collisions.push_back(L_info);
    //         }
    //     }
    // }

    // Resolve collisions
    for (int i = 0; i < m_Collisions.size(); ++i)
	{
        PhysicsSystem::CollisionDetection::ImpulseCollisionResolution(&m_Collisions[i]);
    }

    m_Collisions.clear();
}

void Scene::HandleInputs(const Uint8* _inputKeys)
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.25f);
	glm::vec3 leftMove = glm::vec3(0.25f, 0.0f, 0.0f);
	glm::vec3 rotateY = glm::vec3(0.0f, 0.25f, 0.0f);

    if (_inputKeys[SDL_SCANCODE_A])
	{
		m_Camera.Rotate(rotateY);
	}

	if (_inputKeys[SDL_SCANCODE_D])
	{
		m_Camera.Rotate(-rotateY);
	}
		
	if (_inputKeys[SDL_SCANCODE_W])
	{
		m_Camera.Translate(forward);
	}

	if (_inputKeys[SDL_SCANCODE_S])
	{
		m_Camera.Translate(-forward);
	}

	if(_inputKeys[SDL_SCANCODE_LEFT])
	{
		m_Entities[0]->Move(leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_RIGHT])
	{
		m_Entities[0]->Move(-leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_UP])
	{
		m_Entities[0]->Move(forward);
	}

	if(_inputKeys[SDL_SCANCODE_DOWN])
	{
		m_Entities[0]->Move(-forward);
	}

	// if (_inputKeys[SDL_SCANCODE_X])
	// {
	// 	m_Entities[0].Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	// }

	// if (_inputKeys[SDL_SCANCODE_Z])
	// {
	// 	m_Entities[0].Scale(glm::vec3(-0.1f, -0.1f, -0.1f));
	// }
}