#include "Pipeline/Object.h"
#include "Pipeline/Model.h"
#include "Pipeline/Texture.h"
#include "Pipeline/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Object::Object(const char* _modelPath, const char* _texturePath)
{
    m_Model = std::make_shared<Model>(_modelPath);
    m_Texture = std::make_shared<Texture>(_texturePath);

	m_Coords = glm::vec3(1.0f, 1.0f, 1.0f);
	m_MatrixCoords = glm::mat4(1.0f);
}

Object::~Object()
{

}

void Object::Draw(ShaderProgram* _shader)
{
    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->id());

	_shader->SetUniform("u_Model", m_MatrixCoords);

	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertexCount());

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::Move(glm::vec3 _movement)
{
	m_Coords += _movement;

	m_MatrixCoords = glm::translate(m_MatrixCoords, _movement);
}