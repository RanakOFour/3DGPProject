#include "GLPipeline/Scene/Scene.h"
#include "SDL2/SDL.h"

Scene::Scene() :
    m_Entities(),
    m_Camera()
{

}

Scene::~Scene()
{

}

void Scene::AddEntity(Entity* _entity)
{
	_entity->SetID(m_Entities.size());
    m_Entities.push_back(*_entity);
}

void Scene::Update(const uint8_t* _inputKeys)
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
		m_Entities[0].Move(leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_RIGHT])
	{
		m_Entities[0].Move(-leftMove);
	}

	if(_inputKeys[SDL_SCANCODE_UP])
	{
		m_Entities[0].Move(forward);
	}

	if(_inputKeys[SDL_SCANCODE_DOWN])
	{
		m_Entities[0].Move(-forward);
	}

	if (_inputKeys[SDL_SCANCODE_X])
	{
		m_Entities[0].Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	}

	if (_inputKeys[SDL_SCANCODE_Z])
	{
		m_Entities[0].Scale(glm::vec3(-0.1f, -0.1f, -0.1f));
	}

	for(int i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i].Update(&m_Entities);
	}

    for (int i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i].Draw(&m_Camera);
	}

	glUseProgram(0);
}