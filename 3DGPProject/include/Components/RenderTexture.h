#ifndef RENDERTEXTURE_H

#define RENDERTEXTURE_H

#include "GL/glew.h"
#include "glm/ext.hpp"
#include <vector>

class RenderTexture
{
private:
	GLuint m_id, m_frameBufferId, m_renderBufferId;
	int m_width, m_height;
	bool m_dirty;
	std::vector<unsigned char> m_data;

public:
	RenderTexture(int _w, int _h);
	RenderTexture(const char* _path);
	~RenderTexture();

	void Bind();
	void Unbind();

	GLuint GetID();
};

#endif