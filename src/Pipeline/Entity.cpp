#include "Pipeline/Entity.h"
#include "Pipeline/Model.h"
#include "Pipeline/Texture.h"
#include "Pipeline/ShaderProgram.h"
#include "Pipeline/Camera.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Entity::Entity(const char* _modelPath, const char* _texturePath) :
	m_Model(_modelPath),
	m_Texture(_texturePath),
	m_Transform()
{
	printf("Entity constructor\n");
}

Entity::~Entity()
{

}

void Entity::Draw(ShaderProgram* _shaderProgram)
{
    glBindVertexArray(m_Model.GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture.id());

	_shaderProgram->SetUniform("u_Model", m_Transform.GetModelMatrix());

	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model.GetVertexCount());

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