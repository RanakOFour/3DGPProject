#include <stdexcept>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

#include "Pipeline/Texture.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/ShaderProgram.h"

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
	const std::vector<glm::vec3> positions= {
											{-0.5f, 0.5f, 0.0f},
											{0.5f, -0.5f, 0.0f},
											{0.5f, 0.5f, 0.0f},
											{0.5f, -0.5f, 0.0f},
											{-0.5f, 0.5f, 0.0f},
											{-0.5f, -0.5f, 0.0f}
	};

	VertexBuffer* positionBuffer = new VertexBuffer(3);

	for (int i = 0; i < positions.size(); i++)
	{
		positionBuffer->Add(positions[i]);
	}
	printf("Positions uploaded\n");

	VertexArray* vertexArray = new VertexArray();
	vertexArray->SetBuffer("something", positionBuffer);

	printf("VAO Uploaded and positions VBO bound\n");

	// Setup colors
	const std::vector<glm::vec2> textureCoords = {
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f}
	};

	VertexBuffer* texCoordBuffer = new VertexBuffer(2);

	for (int i = 0; i < textureCoords.size(); i++)
	{
		texCoordBuffer->Add(textureCoords[i]);
	}

	vertexArray->SetBuffer("something", texCoordBuffer);

	printf("Colours VBO created and bound to VAO\n");

	// Vertex shader code
	const GLchar* vertexShaderSrc =
		"attribute vec3 a_Position;            " \
		"attribute vec2 a_PixelColor;               " \
		"uniform mat4 u_Projection;			   " \
		"uniform mat4 u_View;" \
		"uniform mat4 u_Model;				   " \
		"varying vec2 v_texCoord;                 " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0); " \
		" v_texCoord = a_PixelColor;                  "\
		"}";

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

	ShaderProgram* shaderProgram = new ShaderProgram(vertexShaderSrc, fragmentShaderSrc);

	Texture* texture = new Texture("./test.png");

	glEnable(GL_BLEND);

	// Wireframe
	//glPolygonMode(GL_FRONT, GL_LINE);

	float rotSpeed = 0.5f;
	int windowWidth, windowHeight;
	bool quit = false;

	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	glm::mat4 viewMatrix = glm::mat4(1.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -2.5f));

	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.25f);
	glm::vec3 left = glm::vec3(0.25f, 0.0f, 0.0f);

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

		const Uint8* key = SDL_GetKeyboardState(NULL);
		if (key[SDL_SCANCODE_A])
		{
			viewMatrix = glm::translate(viewMatrix, left);
		}

		if (key[SDL_SCANCODE_D])
		{
			viewMatrix = glm::translate(viewMatrix, -left);
		}
		
		if (key[SDL_SCANCODE_W])
		{
			viewMatrix = glm::translate(viewMatrix, forward);
		}

		if (key[SDL_SCANCODE_S])
		{
			viewMatrix = glm::translate(viewMatrix, -forward);
		}

		if (key[SDL_SCANCODE_LEFT])
		{
			viewMatrix = glm::rotate(viewMatrix, 0.00872665f, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (key[SDL_SCANCODE_RIGHT])
		{
			viewMatrix = glm::rotate(viewMatrix, -0.00872665f, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Instruct OpenGL to use our shader program, VAO and texture
		shaderProgram->Use();
		glBindVertexArray(vertexArray->GetID());
		glBindTexture(GL_TEXTURE_2D, texture->id());


		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);



		//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotSpeed), glm::vec3(0, 1, 0));


		// Make sure the current program is bound

		// Upload the model matrix
		shaderProgram->SetUniform("u_Model", modelMatrix);
		shaderProgram->SetUniform("u_View", viewMatrix);
		shaderProgram->SetUniform("u_Projection", projectionMatrix);

		// Draw shape
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_GL_SwapWindow(window);
	}
}