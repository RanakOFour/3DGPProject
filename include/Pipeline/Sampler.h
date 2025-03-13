#include <GL/glew.h>

#pragma once

class Texture;

// NOT IMPLEMENTED
class Sampler
{
	GLuint m_id;
	Texture* m_texture;

	public:
	Sampler(Texture* _texture);
	~Sampler();

	GLuint GetID();
};