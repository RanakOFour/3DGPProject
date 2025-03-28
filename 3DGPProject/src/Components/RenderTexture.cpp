#include "Components/RenderTexture.h"
#include "GL/glew.h"
#include "glm/ext.hpp"
#include "stb_image.h"

RenderTexture::RenderTexture(int _w, int _h) :
	m_id(0),
	m_frameBufferId(0),
	m_renderBufferId(0),
	m_width(_w),
	m_height(_h),
	m_dirty(true),
	m_data()
{

}

RenderTexture::RenderTexture(const char* _path) :
	m_id(0),
	m_frameBufferId(0),
	m_renderBufferId(0),
	m_width(0),
	m_height(0),
	m_dirty(true),
	m_data()
{
	unsigned char* data = stbi_load(_path, &m_width, &m_height, NULL, 4);

	if (!data || data == NULL)
	{
		printf("Failed to load texture\n");
		throw std::exception();
	}

	m_data.assign(data, data + m_width * m_height * 4);

	// Free the loaded data because we now have a copy on the GPU
	free(data);
}

RenderTexture::~RenderTexture()
{
	if (m_id != 0)
	{
		glDeleteTextures(1, &m_id);
	}

	if (m_frameBufferId != 0)
	{
		glDeleteFramebuffers(1, &m_frameBufferId);
	}
	
	if (m_renderBufferId != 0)
	{
		glDeleteRenderbuffers(1, &m_renderBufferId);
	}
}

void RenderTexture::Bind()
{
	if (m_dirty)
	{
		glGenFramebuffers(1, &m_frameBufferId);
		if (!m_frameBufferId)
		{
			throw std::exception();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		if (m_data.size() < 1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);

		glGenRenderbuffers(1, &m_renderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
}

void RenderTexture::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderTexture::GetID()
{
	return m_id;
}