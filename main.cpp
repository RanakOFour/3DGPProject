#include <stdexcept>
#include <glm/ext.hpp>

#include "Pipeline/SDLGLWindow.h"
#include "Pipeline/Entity.h"

#undef main

int main()
{
	SDLGLWindow window("Test", 800, 600);

	Entity catWarrior("./resources/curuthers/curuthers.obj", 
					  "./resources/curuthers/Whiskers_diffuse.png");

	window.AddEntity(&catWarrior);

	while (!window.m_Quit)
	{
		window.Update();
	}
}