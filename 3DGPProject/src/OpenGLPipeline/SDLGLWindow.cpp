#include "OpenGLPipeline/SDLGLWindow.h"
#include "OpenGLPipeline/Entity.h"
#include "OpenGLPipeline/Camera.h"
#include "OpenGLPipeline/RenderTexture.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

#include <memory>

#include <stdexcept>
#include <iostream>


SDLGLWindow::SDLGLWindow(const char* _title, int _width, int _height) :
    m_Window(nullptr),
    m_Entities(),
	m_camera(nullptr),
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

	m_camera = new Camera();

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    printf("Window created\n");

	blue = new RenderTexture(glm::ivec2(800, 800));
}

SDLGLWindow::~SDLGLWindow()
{
    SDL_DestroyWindow(m_Window);
	delete m_camera;
}

void SDLGLWindow::Update()
{
    // Handle input
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.25f);
	glm::vec3 leftMove = glm::vec3(0.25f, 0.0f, 0.0f);
	glm::vec3 rotateY = glm::vec3(0.0f, 0.25f, 0.0f);

    SDL_Event e = { 0 };

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_Quit = true;
		}
	}

	const Uint8* key = SDL_GetKeyboardState(NULL);
	if (key[SDL_SCANCODE_A])
	{
		m_camera->Rotate(rotateY);
	}

	if (key[SDL_SCANCODE_D])
	{
		m_camera->Rotate(-rotateY);
	}
		
	if (key[SDL_SCANCODE_W])
	{
		m_camera->Translate(forward);
	}

	if (key[SDL_SCANCODE_S])
	{
		m_camera->Translate(-forward);
	}

	if(key[SDL_SCANCODE_LEFT])
	{
		m_Entities[0]->Move(leftMove);
	}

	if(key[SDL_SCANCODE_RIGHT])
	{
		m_Entities[0]->Move(-leftMove);
	}

	if(key[SDL_SCANCODE_UP])
	{
		m_Entities[0]->Move(forward);
	}

	if(key[SDL_SCANCODE_DOWN])
	{
		m_Entities[0]->Move(-forward);
	}

	if (key[SDL_SCANCODE_X])
	{
		m_Entities[0]->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	}

	if (key[SDL_SCANCODE_Z])
	{
		m_Entities[0]->Scale(glm::vec3(-0.1f, -0.1f, -0.1f));
	}

    // Render
    int L_windowWidth, L_windowHeight;
    SDL_GetWindowSize(m_Window, &L_windowWidth, &L_windowHeight);
	glViewport(0, 0, L_windowWidth, L_windowHeight);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i]->Draw(m_camera);
	}

	glUseProgram(0);

	SDL_GL_SwapWindow(m_Window);
}

void SDLGLWindow::AddEntity(Entity* _object)
{
    std::shared_ptr<Entity> L_newEntityPtr(_object);
    m_Entities.push_back(L_newEntityPtr);

    printf("Entity added\n");
}