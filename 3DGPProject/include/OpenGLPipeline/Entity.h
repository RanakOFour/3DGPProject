#ifndef ENTITY_H

#define ENTITY_H

#include "OpenGLPipeline/Model.h"
#include "OpenGLPipeline/ShaderProgram.h"
#include "OpenGLPipeline/Transform.h"
#include "OpenGLPipeline/Texture.h"
#include "OpenGLPipeline/Camera.h"

#include <memory>
#include <vector>
#include <glm/ext.hpp>

class Entity 
{
    Transform m_Transform;
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Texture> m_Texture;
    std::vector<std::shared_ptr<ShaderProgram>> m_Shaders;

    public:
    Entity(const char* _modelPath, const char* _texturePath);
    Entity(glm::vec3 _location);
    ~Entity();

    void Draw(Camera* _camera);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
};

#endif