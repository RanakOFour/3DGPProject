#ifndef OPENGLERROR_H

#define OPENGLERROR_H

#include "GL/glew.h"
#include "glm/ext.hpp"
#include "Pipeline/ShaderProgram.h"
#include <vector>

enum ErrorType
{
	Buffer,
	Shader,
	Array,
	Texture
};

class OpenGLError
{
public:

	static void DisplayError(ErrorType _type, void* _data)
	{
		GLint L_maxLength{ 0 };
		std::vector<GLchar> L_errorLog;

		switch (_type)
		{
		case Shader:
			GLuint* _shaderId = (GLuint*)_data;
			glGetShaderiv(*_shaderId, GL_INFO_LOG_LENGTH, &L_maxLength);

			L_errorLog = std::vector<GLchar>(L_maxLength);
			glGetShaderInfoLog(*_shaderId, L_maxLength, &L_maxLength, &L_errorLog[0]);

			glDeleteShader(*_shaderId);
		}

		printf("OpenGLError: %s", &L_errorLog[0]);
	}
};


#endif
