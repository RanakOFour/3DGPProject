#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GLPipeline/VertexArray.h"
#include "GLPipeline/Texture.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>

class ShaderProgram
{
	GLuint m_id;


	public:
	ShaderProgram(const std::string& _vertexPath, const std::string& _fragPath);
	~ShaderProgram();

	void Use();
	void SetUniform(const std::string& _uniformName, glm::mat4 _value);
	void SetUniform(const std::string& _uniformName, glm::vec4 _value);
	void SetUniform(const std::string& _uniformName, glm::vec3 _value);
	void SetUniform(const std::string& _uniformName, glm::vec2 _value);
	void SetUniform(const std::string& _uniformName, float _value);

	GLuint GetID();
};

#endif