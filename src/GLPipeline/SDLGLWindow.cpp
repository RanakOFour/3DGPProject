#include "GLPipeline/SDLGLWindow.h"
#include "Game/Game.h"
#include "Game/Scene.h"
#include "GLPipeline/OpenGLError.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <SDL2/SDL_events.h>
#include <memory>

#include <stdexcept>
#include <iostream>


SDLGLWindow::SDLGLWindow(const char* _title, int _width, int _height) :
    m_Window(nullptr),
	m_Width(_width),
	m_Height(_height),
    m_Quit(false),
	m_MouseInfo()
{
	printf("Creating window\n");
    m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
						 _width, _height, SDL_WINDOW_OPENGL);
	
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

	// Attach error logging function
	OpenGLError::Init();

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LESS);

    printf("Window created\n");
}

SDLGLWindow::~SDLGLWindow()
{
    SDL_DestroyWindow(m_Window);
}

void SDLGLWindow::SetGame(std::shared_ptr<Game> _game)
{
	m_Game = std::weak_ptr<Game>(_game);
}

const Uint8* SDLGLWindow::Update()
{
	SDL_Event e = { 0 };

	m_MouseInfo.deltas.x = 0.0f;
	m_MouseInfo.deltas.y = 0.0f;
	m_MouseInfo.deltas.z = 0.0f;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_Quit = true;
		}

		if(e.type == SDL_MOUSEWHEEL)
		{
			m_MouseInfo.deltas.z = e.wheel.preciseY;
		}

		if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
		{
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				m_MouseInfo.LMBDown = true;
			}
			else
			{
				m_MouseInfo.RMBDown = true;
			}
		}
		else if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
		{
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				m_MouseInfo.LMBDown = false;
			}
			else
			{
				m_MouseInfo.RMBDown = false;
			}
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			m_MouseInfo.deltas.x = e.motion.xrel;
			m_MouseInfo.deltas.y = e.motion.yrel;
		}
	}

	SDL_GetMouseState(&m_MouseInfo.position.x, &m_MouseInfo.position.y);

	// Resize screen
	int L_windowWidth, L_windowHeight;
    SDL_GetWindowSize(m_Window, &L_windowWidth, &L_windowHeight);
	glViewport(0, 0, L_windowWidth, L_windowHeight);

	glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Return keyboard state
	return SDL_GetKeyboardState(NULL);
}