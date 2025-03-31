#include "GLPipeline/Entity/Entity.h"
#include "GLPipeline/Entity/Model.h"
#include "GLPipeline/Entity/Texture.h"
#include "GLPipeline/Entity/ShaderProgram.h"
#include "GLPipeline/Scene/Camera.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Entity::Entity(const char* _modelPath, const char* _texturePath) :
	m_Transform(),
	m_Model(),
	m_Texture(),
	m_Shaders()
{
	printf("Entity constructor\n");

	m_Model = std::make_shared<Model>(_modelPath);
	m_Texture = std::make_shared<Texture>(_texturePath);
	m_Shaders.push_back(std::make_shared<ShaderProgram>("./resources/shaders/default/vert.vs", "./resources/shaders/default/frag.fs"));
}

Entity::~Entity()
{

}

void Entity::Draw(Camera* _camera)
{
    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());

	glm::mat4 L_modelMatrix = m_Transform.GetModelMatrix();

	for(int i = 0; i < m_Shaders.size(); i++)
	{
		m_Shaders[i]->Use();
		_camera->Use(m_Shaders[i].get());
		m_Shaders[i]->SetUniform("u_Model", L_modelMatrix);

		// Draw shape
		glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertexCount());
	}

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Entity::Move(glm::vec3 _movement)
{
	m_Transform.Translate(_movement);
}

void Entity::Rotate(glm::vec3 _rotation)
{
	m_Transform.Rotate(_rotation);
}

void Entity::Scale(glm::vec3 _scale)
{
	m_Transform.Scale(_scale);
}