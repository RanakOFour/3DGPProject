#include "GLPipeline/SDLGLWindow.h"

#include "GLPipeline/Entity/Entity.h"
#include "GLPipeline/Scene/Camera.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

#include <memory>

#include <stdexcept>
#include <iostream>


SDLGLWindow::SDLGLWindow(const char* _title, int _width, int _height) :
    m_Window(nullptr),
	m_Scene(nullptr),
    m_Quit(false)
{
	printf("Creating window\n");
    m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_OPENGL);
    if(m_Window == nullptr)
    {
        printf("Could not create SDL Window\n");
        throw std::runtime_error("Jeepers");
    }
    printf("SDLWindow created\n");

    if (!SDL_GL_CreateContext(m_Window))
	{
        printf("Failed to create GL context\n");
		throw std::runtime_error("Damn");
	}	

	if (glewInit() != GLEW_OK)
	{
        printf("Failed to start GLEW\n");
		throw std::runtime_error("Jinkies");
	}

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    printf("Window created\n");
}

SDLGLWindow::~SDLGLWindow()
{
    SDL_DestroyWindow(m_Window);
}

void SDLGLWindow::Update()
{
	SDL_Event e = { 0 };

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_Quit = true;
		}
	}

	if(m_Scene != nullptr)
	{
		const Uint8* key = SDL_GetKeyboardState(NULL);

		m_Scene->HandleSDLInput(key);

    	// Render
    	int L_windowWidth, L_windowHeight;
    	SDL_GetWindowSize(m_Window, &L_windowWidth, &L_windowHeight);
		glViewport(0, 0, L_windowWidth, L_windowHeight);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Scene->Update();

		SDL_GL_SwapWindow(m_Window);
	}
	else
	{
		printf("SDLGLWindow does not have a scene set!\n");
	}
}

void SDLGLWindow::SetScene(Scene* _scene)
{
    m_Scene = _scene;
}