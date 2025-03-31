#ifndef SCENE_H

#define SCENE_H

#include "GLPipeline/Entity/Entity.h"
#include "GLPipeline/Scene/Camera.h"

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
	void Update(const uint8_t* _inputKeys);
};

#endif