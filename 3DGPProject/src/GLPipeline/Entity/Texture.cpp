#include "GLPipeline/Entity/Texture.h"

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
	//stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(_path.c_str(), &m_size.x, &m_size.y, NULL, 4);

	if (!data || data == NULL)
	{
		printf("Failed to load texture\n");
		throw std::exception();
	}

	m_data.assign(data, data + m_size.x * m_size.y * 4);

	// Free the loaded data because we now have a copy on the GPU
	free(data);
}


Texture::Texture(glm::ivec2 _size) :
	m_dirty(true),
	m_data(),
	m_size(_size),
	m_id(0)
{

}

Texture::~Texture()
{
	// delete data in vbo
	glDeleteTextures(1, &m_id);
}

GLuint Texture::GetID()
{
	if (m_dirty)
	{
		if (!m_id)
		{
			glGenTextures(1, &m_id);
		}

		glBindTexture(GL_TEXTURE_2D, m_id);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);

		printf("Texture %d cleaned\n", m_id);

		m_dirty = false;
	}

	return m_id;
}

void Texture::Size(glm::ivec2 _size)
{
	m_size = _size;

	// Change size of vbo on gpu
}

const glm::ivec2 Texture::Size()
{
	return m_size;
}

void Texture::Load(const std::string& _path)
{
	if (!m_dirty)
	{
		glDeleteBuffers(1, &m_id);
	}

	unsigned char* data = stbi_load(_path.c_str(), &m_size.x, &m_size.y, NULL, 4);

	if (!data || data == NULL)
	{
		printf("Texture::Load -> Data to load is NULL or invalid!\n");
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

	m_dirty = true;
}

void Texture::Pixel(glm::ivec2 _position, glm::vec4& _color)
{
	// Put data from _color into pixel at _position
	unsigned char* colStart = &m_data[m_size.y * _position.y * _position.x];

	colStart[0] = _color[0];
	colStart[1] = _color[1];
	colStart[2] = _color[2];
	colStart[3] = _color[3];
}

const glm::vec4 Texture::Pixel(glm::ivec2 _position)
{
	unsigned char* colStart = &m_data[m_size.y * _position.y * _position.x];

	return glm::vec4(colStart[0], colStart[1], colStart[2], colStart[3]);
}