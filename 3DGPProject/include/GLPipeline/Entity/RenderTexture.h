#ifndef RENDERTEXTURE_H

#define RENDERTEXTURE_H

#include "GLPipeline/Entity/RenderTexture.h"
#include "GLPipeline/Entity/Texture.h"
#include "GLPipeline/Entity/ShaderProgram.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <vector>
#include <string>


class RenderTexture : public Texture
{
private:
	GLuint m_frameBufferId, m_renderBufferId;
	
public:
	RenderTexture(glm::ivec2 _size);
	~RenderTexture();

	void Bind();
	void Unbind();
	void Load(const std::string& _path);
};

#endif