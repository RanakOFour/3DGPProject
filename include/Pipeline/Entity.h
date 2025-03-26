#ifndef ENTITY_H

#define ENTITY_H
#include "Pipeline/Model.h"
#include "Pipeline/Texture.h"
#include "Pipeline/ShaderProgram.h"
#include "Components/Transform.h"
#include <memory>
#include <glm/ext.hpp>

#pragma once

class Entity 
{
    Model m_Model;
    Texture m_Texture;
    Transform m_Transform;

    public:
    Entity(const char* _modelPath, const char* _texturePath);
    ~Entity();

    void Draw(ShaderProgram* _shader);
    void Move(glm::vec3 _movement);
};

#endif