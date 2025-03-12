#include "Pipeline/Texture.h"
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(const std::string& _path) :
	m_dirty(true),
	m_data(),
	m_size(0, 0),
	m_id(0)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(_path.c_str(), &m_size.x, &m_size.y, NULL, 4);

	if (!data || data == NULL)
	{
		printf("Failed to load texture\n");
		throw std::exception();
	}

	printf("Interating by %d on X and %d on Y\n", m_size.x, m_size.y);

	unsigned char* currentChannel = data;

	for (int i = 0; i < m_size.y; i++)
	{
		for (int j = 0; j < m_size.x; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				m_data.push_back(*currentChannel);
				++currentChannel;
			}
		}
	}

	// Free the loaded data because we now have a copy on the GPU
	free(data);
}


Texture::Texture(glm::ivec2 _size) :
	m_dirty(true),
	m_data(),
	m_size(_size),
	m_id(-1)
{

}

Texture::~Texture()
{
	// delete data in vbo
}

GLuint Texture::id()
{
	if (m_dirty)
	{
		glGenTextures(1, &m_id);

		glBindTexture(GL_TEXTURE_2D, m_id);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[0]);

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);

		printf("Texture %d cleaned\n", m_id);
		m_dirty = false;
	}

	return m_id;
}

void Texture::size(glm::ivec2 _size)
{
	m_size = _size;

	// Change size of vbo on gpu
}

const glm::ivec2 Texture::size()
{
	return m_size;
}

void Texture::load(const std::string& _path)
{
	if (m_id != -1)
	{
		// Delete data on buffer
		m_dirty = false;
	}

	unsigned char* data = stbi_load(_path.c_str(), &m_size.x, &m_size.y, NULL, 4);

	if (!data || data == NULL)
	{
		throw std::exception();
	}

	for (int i = 0; i < m_size.y; i++)
	{
		for (int j = 0; j < m_size.x; j++)
		{
			int newCol = 0;

			// Iterate once for each channel (4)
			for (int k = 0; k < 4; k++)
			{

				int placesToShift = 32 - (8 * k + 1);

				// Put channel's color bits into the correct place
				newCol += (int)*data << placesToShift;
				++data;
			}

			m_data.push_back((float)newCol);
		}
	}

	// Create and bind a texture.
	glGenTextures(1, &m_id);

	if (!m_id)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, data);

	// Free the loaded data because we now have a copy on the GPU
	free(data);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

	// set m_data data
}

void Texture::pixel(glm::ivec2 _position, glm::vec4& _color)
{
	// Put data from _color into pixel at _position
}

const glm::vec4 Texture::pixel(glm::ivec2 _position)
{
	// Return color of pixel at _position
	int pixel = (int)m_data[_position.x * m_size.x + _position.y];
	unsigned char r = pixel >> 24;
	unsigned char g = pixel >> 16;
	unsigned char b = pixel >> 8;
	unsigned char a = pixel;


	return glm::vec4(r, g, b, a);
}