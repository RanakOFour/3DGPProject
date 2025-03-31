#ifndef SCENE_H

#define SCENE_H

#include "OpenGLPipeline/Entity.h"
#include "OpenGLPipeline/Camera.h"
#include <vector>

#pragma once
class Scene
{
private:
	std::vector<Entity> m_Entities;
	Camera m_Camera;

public:
	Scene();
	~Scene();

	void AddEntity(Entity* _entity);
	void HandleSDLInput(const uint8_t* _inputKeys);
	void Update();
};

#endif