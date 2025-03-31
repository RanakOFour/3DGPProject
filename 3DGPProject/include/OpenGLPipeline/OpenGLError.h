#ifndef OPENGLERROR_H

#define OPENGLERROR_H

#include "GL/glew.h"
#include "glm/ext.hpp"
#include "OpenGLPipeline/ShaderProgram.h"
#include <vector>

/*
	Error logging static for OpenGLObjects
*/
class OpenGLError
{
public:

	/*
		Takes in a type of OpenGL object, and a pointer to said data, then prints the associated error while deleting the associated object
	*/
	static void DisplayError(GLenum _source, GLenum _type, GLuint _id,
							 GLenum _severity, GLsizei _length, const GLchar* _message,
							 const void* _userParam)
	{
		printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			   ( _type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
			   _type, _severity, _message );
	}

	static void Init()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(DisplayError, 0);
		printf("OpenGLError Initialized\n");
	}
};


#endif
