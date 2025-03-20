#include <stdexcept>
#include <glm/ext.hpp>

#include "Pipeline/SDLGLWindow.h"
#include "Pipeline/Object.h"

#undef main

int main()
{
	SDLGLWindow window("Test", 800, 600);

	Object catWarrior("./resources/curuthers/curuthers.obj", 
					  "./resources/curuthers/Whiskers_diffuse.png");

	window.AddObject(&catWarrior);

	while (!window.m_Quit)
	{
		window.Update();
	}
}