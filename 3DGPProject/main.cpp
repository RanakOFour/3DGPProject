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

	glm::vec3 L_catBox(2.0f, 3.0f, 2.0f);

	Entity catWarrior("./resources/curuthers/curuthers.obj", 
					  "./resources/curuthers/Whiskers_diffuse.png",
					  true,
					  L_catBox);

	Entity catWarrior2("./resources/curuthers/curuthers.obj", 
					   "./resources/curuthers/Whiskers_diffuse.png",
					   false,
					   L_catBox);

	catWarrior2.Move(glm::vec3(0.0f, 0.0f, 1.0f));
	Scene scene;
	scene.AddEntity(&catWarrior);
	scene.AddEntity(&catWarrior2);

	catWarrior2.Move(glm::vec3(0.0f, 0.0f, 1.0f));

	window.SetScene(&scene);

	while (!window.m_Quit)
	{
		window.Update();
		std::system("clear");
	}
}