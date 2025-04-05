#include <stdexcept>
#include <glm/ext.hpp>

#include "GLPipeline/SDLGLWindow.h"
#include "GLPipeline/Scene/Scene.h"
#include "GLPipeline/Entity/Entity.h"
#include "GLPipeline/OpenGLError.h"

#undef main

int main()
{
	SDLGLWindow window("Test", 800, 600);

	OpenGLError::Init();

	Entity catWarrior("./resources/curuthers/curuthers.obj", 
					  "./resources/curuthers/Whiskers_diffuse.png");

	Entity catWarrior2("./resources/curuthers/curuthers.obj", 
					   "./resources/curuthers/Whiskers_diffuse.png");

	Scene scene;
	scene.AddEntity(&catWarrior);
	scene.AddEntity(&catWarrior2);

	window.SetScene(&scene);

	while (!window.m_Quit)
	{
		window.Update();
	}
}