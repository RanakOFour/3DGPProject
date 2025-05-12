#include "Game/GameEntity.h"
#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"
#include "Physics/Transform.h"
#include "Physics/Physics.h"
#include "Game/Camera.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

GameEntity::GameEntity(const char* _modelPath, const char* _texturePath, glm::vec3 _pos)
{
	printf("GameEntity constructor\n");
	m_id = -1;
	m_Transform = std::make_shared<Transform>();
	m_Model = std::make_shared<Model>(_modelPath);
	m_Shader = std::make_shared<ShaderProgram>("./resources/shaders/specular/vert.vs", "./resources/shaders/specular/frag.fs");
	m_Texture = std::make_unique<Texture>(_texturePath);
	m_Physics = std::make_shared<Physics>(m_Model, m_Transform);
}

GameEntity::~GameEntity()
{

}

void GameEntity::Update(float _delta)
{
	m_Physics->Update(_delta);
}

void GameEntity::Draw(Camera* _camera)
{
	printf("Drawing entity %d\n", m_id);

	m_Shader->Use();
	_camera->Use(m_Shader.get());

    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());

	glm::mat4 L_modelMatrix = glm::translate(glm::mat4(1.0f), m_Transform->GetPosition());

	L_modelMatrix *= glm::mat4_cast(m_Transform->GetRotation());

	L_modelMatrix = glm::scale(L_modelMatrix, m_Transform->GetScale());

	m_Shader->Use();
	_camera->Use(m_Shader.get());
	m_Shader->SetUniform("u_Model", L_modelMatrix);

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

std::weak_ptr<CollisionShape> GameEntity::GetCollider()
{
	return std::weak_ptr<CollisionShape>(m_Collider);
}