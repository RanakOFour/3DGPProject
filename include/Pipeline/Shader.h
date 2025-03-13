#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>

class Shader
{
	GLuint m_id;
	unsigned int m_shaderType;
	std::string m_shaderText;
	bool m_compiled;

	public:
	Shader(const std::string& _text, unsigned int _shaderType);
	~Shader();

	GLuint GetID();
	bool Compile();
};