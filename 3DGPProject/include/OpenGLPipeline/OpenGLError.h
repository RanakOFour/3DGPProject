#ifndef OPENGLERROR_H

#define OPENGLERROR_H

#include "GL/glew.h"
#include "glm/ext.hpp"
#include "OpenGLPipeline/ShaderProgram.h"
#include <vector>

enum ErrorType
{
	Buffer,
	Shader,
	Array,
	Texture
};

/*
	Error logging static for OpenGLObjects
*/
class OpenGLError
{
public:

	/*
		Takes in a type of OpenGL object, and a pointer to said data, then prints the associated error while deleting the associated object
	*/
	static void DisplayError(ErrorType _type, GLuint* _data)
	{
		GLint L_maxLength{ 0 };
		std::vector<GLchar> L_errorLog;

		switch (_type)
		{
		case Shader:
			glGetShaderiv(*_data, GL_INFO_LOG_LENGTH, &L_maxLength);

			L_errorLog = std::vector<GLchar>(L_maxLength);
			glGetShaderInfoLog(*_data, L_maxLength, &L_maxLength, &L_errorLog[0]);

			glDeleteShader(*_data);
		}

		printf("OpenGLError: %s", &L_errorLog[0]);
	}
};


#endif
