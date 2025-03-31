#include <stdexcept>
#include <glm/ext.hpp>

#include "OpenGLPipeline/SDL2/SDLGLWindow.h"
#include "OpenGLPipeline/Scene.h"
#include "OpenGLPipeline/Entity.h"
#include "OpenGLPipeline/OpenGLError.h"

#undef main

int main()
{
	SDLGLWindow window("Test", 800, 600);

	OpenGLError::Init();

	Entity catWarrior("./resources/curuthers/curuthers.obj", 
					  "./resources/curuthers/Whiskers_diffuse.png");

	Scene scene;
	scene.AddEntity(&catWarrior);

	window.SetScene(&scene);

	while (!window.m_Quit)
	{
		window.Update();
	}
}