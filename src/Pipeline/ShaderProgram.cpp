#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Pipeline/ShaderProgram.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/OpenGLError.h"

ShaderProgram::ShaderProgram(const std::string& _vertexPath, const std::string& _fragPath)
{
	// Load vertex and frag shader text from file paths

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(_vertexPath);
		fShaderFile.open(_fragPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//printf("Vertex Shader: %s\n Frag Shader: %s\n", vShaderCode, fShaderCode);

	printf("Pre vertex shader\n");

	// Create a new vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// Attach shader code
	glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
	// Compile shader
	glCompileShader(vertexShaderId);

	// Get success val for shader program compilation
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		OpenGLError::DisplayError(ErrorType::Shader, (void*)&vertexShaderId);
		throw std::exception();
	}
	printf("Vertex shader created\n");

	// Create a new fragment shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	// Upload shader code to GPU
	glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);
	// Compiler fragment shader
	glCompileShader(fragmentShaderId);
	// Get success val for shader compilation
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		OpenGLError::DisplayError(ErrorType::Shader, (void*)&fragmentShaderId);
		throw std::exception();
	}
	printf("Vertex frag created\n");

	// Create new shader program and attach shader objects
	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShaderId);
	glAttachShader(m_id, fragmentShaderId);

	// Associated values in VBOs to variables in the shader code
	glBindAttribLocation(m_id, 0, "a_Position");
	glBindAttribLocation(m_id, 1, "a_PixelColor");

	// Perform the link and check for failure
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf("Failed to link shader program\n");
		throw std::exception();
	}

	printf("Shader program created\n");

	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(m_id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(m_id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}

ShaderProgram::~ShaderProgram()
{
	if(m_id != 0)
	{
		glDeleteProgram(m_id);
	}
}

void ShaderProgram::Use()
{
	glUseProgram(m_id);
}

void ShaderProgram::SetUniform(const std::string& _uniformName, glm::mat4 _value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, _uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(_value));
}

void ShaderProgram::SetUniform(const std::string& _uniformName, glm::vec4 _value)
{
	glUniform4fv(glGetUniformLocation(m_id, _uniformName.c_str()), 4, glm::value_ptr(_value));
}

void ShaderProgram::SetUniform(const std::string& _uniformName, glm::vec3 _value)
{
	glUniform3fv(glGetUniformLocation(m_id, _uniformName.c_str()), 3, glm::value_ptr(_value));
}

void ShaderProgram::SetUniform(const std::string& _uniformName, float _value)
{
	glUniform1f(glGetUniformLocation(m_id, _uniformName.c_str()), _value);
}

GLuint ShaderProgram::GetID()
{
	return m_id;
}