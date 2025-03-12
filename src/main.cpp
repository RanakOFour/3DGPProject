#include <stdexcept>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
// Needs to be defined before the include in exactly
// one compilation unit.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#undef main

int main()
{
	SDL_Window* window = SDL_CreateWindow("Test", 50, 50,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::runtime_error("Damn");
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Glew not ok");
	}

	//Setup positions
	const GLfloat positions[] = {
								 -0.5f, 0.5f, 0.0f,
								 0.5f, -0.5f, 0.0f,
								 0.5f, 0.5f, 0.0f,
								 0.5f, -0.5f, 0.0f,
								 -0.5f, 0.5f, 0.0f,
								 -0.5f, -0.5f, 0.0f
	};


	// Create VBO for positions on the GPU and upload positions into it
	GLuint positionsVboId = 0;
	// Gen buffer and return id of buffer in positionsVboId
	glGenBuffers(1, &positionsVboId);
	if (!positionsVboId)
	{
		throw std::exception();
	}
	// Assign positionVboId as the currently selected Buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
	// Upload a copy of 'positions' from memory into the new VBO
	printf("New size of positions buffer, %d\n", sizeof(positions));
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	printf("Positions uploaded\n");


	// Create VAO
	GLuint vaoId = 0;
	// Create a new VAO on the GPU and returns id to vaoId
	glGenVertexArrays(1, &vaoId);
	if (!vaoId)
	{
		throw std::exception();
	}
	// Bind the new VAO to current context 
	glBindVertexArray(vaoId);
	// Bind the position VBO to current context
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
	// Tells the VAO how to interpret the positionsVBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	printf("GLFloat size: %d\n", sizeof(GLfloat));

	// Enables VAO for use
	glEnableVertexAttribArray(0);
	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	printf("VAO Uploaded and positions VBO bound\n");

	// Setup colors
	const GLfloat textureCoords[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	// Create colors VBO
	GLuint texCoordsVBO = 0;
	glGenBuffers(1, &texCoordsVBO);
	if (!texCoordsVBO)
	{
		throw std::exception();
	}
	//Bind vao to color vbo
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
	// Upload a copy of the data from memory into the colors VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	// Tell VAO the attrib pointer is pos 1, has 4 GL_FLOATS
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	// Enable VBO 1 (colors)
	glEnableVertexAttribArray(1);
	// Reset state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	printf("Colours VBO created and bound to VAO\n");

	// Vertex shader code
	const GLchar* vertexShaderSrc =
		"attribute vec3 a_Position;            " \
		"attribute vec2 a_PixelColor;               " \
		"uniform mat4 u_Projection;			   " \
		"uniform mat4 u_Model;				   " \
		"varying vec2 v_texCoord;                 " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0); " \
		" v_texCoord = a_PixelColor;                  "\
		"}";

	// Create a new vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// Attach shader code
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	// Compile shader
	glCompileShader(vertexShaderId);

	// Get success val for shader program compilation
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printf("Failed to create vertex shader\n");
		throw std::exception();
	}
	printf("Vertex shader created\n");

	// Fragment shader code
	const GLchar* fragmentShaderSrc =
		"varying vec2 v_texCoord;    " \
		"uniform sampler2D u_Texture;"\
		"						   "\
		"void main()               " \
		"{                         " \
		" vec4 tex = texture2D(u_Texture, v_texCoord);" \
		" gl_FragColor = tex; " \
		"}                         ";

	// Create a new fragment shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	// Upload shader code to GPU
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
	// Compiler fragment shader
	glCompileShader(fragmentShaderId);
	// Get success val for shader compilation
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printf("Failed to create frag shader\n");
		throw std::exception();
	}
	printf("Vertex frag created\n");

	// Create new shader program and attach shader objects
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Associated values in VBOs to variables in the shader code
	glBindAttribLocation(programId, 0, "a_Position");
	glBindAttribLocation(programId, 1, "a_PixelColor");

	// Perform the link and check for failure
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf("Failed to link shader program\n");
		throw std::exception();
	}
	printf("Shader program linked\n");

	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	// Find uniform locations in the shader programs
	GLint modelLoc = glGetUniformLocation(programId, "u_Model");
	GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");

	int imageWidth = 0;
	int imageHeight = 0;

	// Load image from file
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load("./test.png", &imageWidth, &imageHeight, NULL, 4);
	if (!data || data == NULL)
	{
		printf("Failed to load image\n");
		throw std::exception();
	}
	printf("Image loaded\n");

	// Create texture on the GPU
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	if (!textureId)
	{
		throw std::exception();
	}

	// Associate GL_TEXTURE_2D with the new texture on GPU
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	printf("Texture uploaded\n");

	// Free the loaded data because we now have a copy on the GPU
	free(data);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT, GL_LINE);

	float angle = 0.5f;
	int windowWidth, windowHeight;
	bool quit = false;

	while (!quit)
	{
		SDL_Event e = { 0 };

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Instruct OpenGL to use our shader program, VAO and texture
		glUseProgram(programId);
		glBindVertexArray(vaoId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Prepare the perspective matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -2.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

		// Increase the float angle so next frame the triangle rotates further
		angle += 0.1f;

		// Make sure the current program is bound

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(projection));

		// Draw shape
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)windowWidth, 0.0f,
			(float)windowHeight, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle
		// would be the size of a single pixel when mapped to an orthographic
		// projection.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, windowHeight - 100, 0));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(projection));

		// Draw shape again
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_GL_SwapWindow(window);
	}
}