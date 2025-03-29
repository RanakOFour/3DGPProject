#ifndef ENTITY_H

#define ENTITY_H

#include "Pipeline/Model.h"
#include "Pipeline/ShaderProgram.h"
#include "Components/Transform.h"
#include "Components/Texture.h"
#include <memory>
#include <glm/ext.hpp>

class Entity 
{
    Model m_Model;
    Transform m_Transform;
    Texture m_Texture;

    public:
    Entity(const char* _modelPath, const char* _texturePath);
    ~Entity();

    void Draw(ShaderProgram* _shader);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
};

#endif