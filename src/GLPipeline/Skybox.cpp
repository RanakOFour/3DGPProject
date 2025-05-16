#include "GLPipeline/Skybox.h"
#include "Game/Camera.h"
#include "Game/Scene.h"

#include "GL/glew.h"

Skybox::Skybox(glm::vec3 _size) :
m_Model("./resources/models/cube.obj"),
m_Program("./resources/shaders/sparks/vert.vs", "./resources/shaders/sparks/frag.fs"),
m_Texture("./resources/textures/floor.jpg"),
m_Size(_size)
{

}

Skybox::~Skybox()
{

}

void Skybox::Draw(glm::mat4 _cameraMatrix, Camera* _camera)
{
	_cameraMatrix *= glm::scale(_cameraMatrix, m_Size);
    // Disable culling so it draws the texture on the inside of the cube
    glDisable(GL_CULL_FACE);

    glBindVertexArray(m_Model.GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetID());

	m_Program.Use();
	_camera->Use(&m_Program, false);
	m_Program.SetUniform("u_Model", _cameraMatrix);
	m_Program.SetUniform("u_Time", _camera->GetScene().lock()->GetTime());
	
	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model.GetVertexCount());

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_CULL_FACE);
}