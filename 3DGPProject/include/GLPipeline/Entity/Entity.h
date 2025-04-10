#ifndef ENTITY_H

#define ENTITY_H

#include "GLPipeline/Entity/Model.h"
#include "GLPipeline/Entity/ShaderProgram.h"
#include "GLPipeline/Entity/Transform.h"
#include "GLPipeline/Entity/Texture.h"
#include "GLPipeline//Entity/Collider/BoxCollider.h"
#include "GLPipeline/Scene/Camera.h"

#include <glm/ext.hpp>
#include <memory>
#include <vector>

class Entity 
{
    int m_id;
    Transform m_Transform;
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Texture> m_Texture;
    std::shared_ptr<ShaderProgram> m_Shader;
    std::shared_ptr<BoxCollider> m_Collider;

    public:
    Entity(const char* _modelPath, const char* _texturePath);
    Entity(glm::vec3 _location);
    ~Entity();

    void Update(std::vector<Entity>* _entityList);
    void Draw(Camera* _camera);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
    void SetID(int _id);
    int GetID();

    BoxCollider* GetBoxCollider();
    void PrintColliderInfo();
};

#endif