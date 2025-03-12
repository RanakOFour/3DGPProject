#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>

#pragma once
class Texture
{
	bool m_dirty;
	std::vector<float> m_data;
	glm::ivec2 m_size;
	GLuint m_id;

public:
	Texture(const std::string& _path);
	Texture(glm::ivec2 _size);
	Texture(const Texture& _copy);
	Texture& operator=(const Texture& _assign);
	~Texture();

	void size(glm::ivec2 _size);
	const glm::ivec2 size();

	void load(const std::string& _path);

	void pixel(glm::ivec2 _position, glm::vec4& _color);
	const glm::vec4 pixel(glm::ivec2 _position);

	GLuint id();
};

