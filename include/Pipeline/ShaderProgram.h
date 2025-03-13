#include <GL/glew.h>
#include <string>
#include <vector>

#pragma once

class Shader;
class ShaderProgram
{
	GLuint m_id;


	public:
		ShaderProgram(const GLchar* _vertex, const GLchar* _frag);
		ShaderProgram(Shader* _vertex, Shader* _frag);
		~ShaderProgram();

		GLuint GetID();
};