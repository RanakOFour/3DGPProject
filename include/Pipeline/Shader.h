#include <GL/glew.h>
#include <glm/ext.hpp>
#include <string>

class Shader
{
	GLuint m_id;
	std::string m_shaderText;
	bool m_compiled;

	public:
	Shader(const std::string& _text);
	~Shader();

	GLuint GetID();
	bool Compile();
};