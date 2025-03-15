#include "Pipeline/Object.h"
#include "Pipeline/Model.h"
#include "Pipeline/Texture.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Object::Object(const char* _modelPath, const char* _texturePath)
{
    m_Model = std::make_shared<Model>(_modelPath);
    m_Texture = std::make_shared<Texture>(_texturePath);
}

Object::~Object()
{

}

void Object::Draw()
{
    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->id());

	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertexCount());

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}