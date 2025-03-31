#include "OpenGLPipeline/RenderTexture.h"
#include "GL/glew.h"
#include "glm/ext.hpp"
#include "stb_image.h"

RenderTexture::RenderTexture(glm::ivec2 _size) : 
	Texture(_size),
	m_frameBufferId(0),
	m_renderBufferId(0)
{

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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);

		glGenRenderbuffers(1, &m_renderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.x, m_size.y);
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

void RenderTexture::Load(const std::string& _path)
{
	printf("You are loading a render texture, you should not be doing this!\n");
}