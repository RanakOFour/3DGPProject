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


public:
	Scene();
	~Scene();

	void AddEntity(Entity* _entity);
	void Update();
};

#endif