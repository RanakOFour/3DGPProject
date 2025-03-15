#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>

#pragma once

class Shader;
class VertexArray;
class Texture;
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
	void SetUniform(const std::string& _uniformName, float _value);

	GLuint GetID();
};