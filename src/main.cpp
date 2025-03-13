#include <stdexcept>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

#include "Pipeline/Model.h"
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

	ShaderProgram shaderProgram("./resources/textureVertex.vs", "./resources/textureFrag.fs");


	Model catModel("./resources/curuthers/curuthers.obj");

	Texture texture("./resources/curuthers/Whiskers_diffuse.png");
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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
			viewMatrix = glm::rotate(viewMatrix, 0.78539816339f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (key[SDL_SCANCODE_RIGHT])
		{
			viewMatrix = glm::rotate(viewMatrix, -0.78539816339f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		// Clear color and depth testing buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Instruct OpenGL to use our shader program, VAO and texture
		shaderProgram.Use();
		glBindVertexArray(catModel.vao_id());
		glBindTexture(GL_TEXTURE_2D, texture.id());


		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);

		//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotSpeed), glm::vec3(0, 1, 0));

		// Upload the matriCES
		shaderProgram.SetUniform("u_Model", modelMatrix);
		shaderProgram.SetUniform("u_View", viewMatrix);
		shaderProgram.SetUniform("u_Projection", projectionMatrix);

		// Draw shape
		glDrawArrays(GL_TRIANGLES, 0, catModel.vertex_count());

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_GL_SwapWindow(window);
	}
}