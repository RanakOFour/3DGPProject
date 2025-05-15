#include "Game/GameEntity.h"
#include "Game/Camera.h"
#include "Game/Scene.h"

#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"

#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Shape/CubeShape.h"
#include "Physics/Shape/SphereShape.h"
#include "Physics/Shape/MeshShape.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

GameEntity::GameEntity(glm::vec3 _position, float _radius, bool _env)
{
	m_id = -1;
	m_Transform = std::make_shared<Transform>();
	m_Transform->SetPosition(_position);
	m_Transform->SetScale(glm::vec3(_radius));

	m_Model = std::make_shared<Model>("./resources/shapes/sphere.obj");
	m_Shader = std::make_shared<ShaderProgram>("./resources/shaders/default/vert.vs", "./resources/shaders/default/frag.fs");
	m_Texture = std::make_shared<Texture>("./resources/curuthers/Whiskers_diffuse.png");

	SphereShape* L_sphere = new SphereShape(_radius, _env);
	m_Collider = std::shared_ptr<CollisionShape>(L_sphere);

	m_Environment = _env;
}

GameEntity::GameEntity(glm::vec3 _position, glm::vec3 _size, bool _env)
{
	m_id = -1;
	m_Transform = std::make_shared<Transform>();
	m_Transform->SetPosition(_position);
	m_Transform->SetScale(glm::vec3(_size));
	
	m_Model = std::make_shared<Model>("./resources/models/cube.obj");
	m_Shader = std::make_shared<ShaderProgram>("./resources/shaders/default/vert.vs", "./resources/shaders/default/frag.fs");
	m_Texture = std::make_unique<Texture>("./resources/textures/floor.jpg");

	CubeShape* L_cube = new CubeShape(_size * 0.5f, _env);
	m_Collider = std::shared_ptr<CollisionShape>(L_cube);

	m_Environment = _env;
}

GameEntity::GameEntity(std::shared_ptr<Model> _model, std::shared_ptr<Texture> _tex, glm::vec3 _pos, glm::vec3 _size, bool _env)
{
	printf("GameEntity constructor\n");
	m_id = -1;
	m_Transform = std::make_shared<Transform>();
	m_Transform->SetPosition(_pos);
	
	m_Model = _model;
	m_Shader = std::make_shared<ShaderProgram>("./resources/shaders/specular/vert.vs", "./resources/shaders/specular/frag.fs");
	m_Texture = _tex;

	CubeShape* L_mesh = new CubeShape(_size * 0.5f, _env);
	m_Collider = std::shared_ptr<CollisionShape>(L_mesh);

	m_Environment = _env;
}

GameEntity::~GameEntity()
{

}

void GameEntity::Update(float _delta)
{
	// Interactable flags?

	printf("ID: %d\nCurrent Pos: %f, %f, %f\n", m_id,
		m_Transform->GetPosition().x, m_Transform->GetPosition().y, m_Transform->GetPosition().z);

	m_Collider->ClearContacts();
}

void GameEntity::Draw(Camera* _camera)
{
	printf("Drawing entity %d\n", m_id);

    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());

	glm::mat4 L_modelMatrix = glm::translate(glm::mat4(1.0f), m_Transform->GetPosition());

	L_modelMatrix *= glm::mat4_cast(m_Transform->GetRotation());

	L_modelMatrix = glm::scale(L_modelMatrix, m_Transform->GetScale());

	m_Shader->Use();
	_camera->Use(m_Shader.get());
	m_Shader->SetUniform("u_Model", m_Transform->ModelMatrix());
	m_Shader->SetUniform("u_Time", m_Scene.lock()->GetTime());
	
	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertexCount());

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameEntity::Move(glm::vec3 _movement)
{
	m_Transform->Move(_movement);
}

void GameEntity::Rotate(glm::vec3 _rotation)
{
	m_Transform->SetRotation(_rotation);
}

void GameEntity::Scale(glm::vec3 _scale)
{
	m_Transform->SetScale(_scale);
}

void GameEntity::SetID(int _id)
{
	printf("ID set to GameEntity as %d\n", _id);
	m_id = _id;
}

int GameEntity::GetID()
{
	return m_id;
}


void GameEntity::SetScene(std::shared_ptr<Scene> _scene)
{ 
	m_Scene = std::weak_ptr<Scene>(_scene); 
	m_Collider->SetParent(m_Scene.lock()->GetEntity(m_id)); 
}