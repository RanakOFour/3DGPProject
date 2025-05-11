#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Game/Camera.h"
#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"
#include "Physics/Physics.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"

#include <glm/ext.hpp>
#include <memory>
#include <vector>

class Scene;
class GameEntity 
{
    // Loop back up
    //std::weak_ptr<Scene> m_Scene;

    // Model & Texture Pool references
    std::shared_ptr<Model> m_Model;
    std::unique_ptr<Texture> m_Texture;

    // Properties
    int m_id;
    Transform m_Transform;
    Physics m_Physics;
    std::shared_ptr<ShaderProgram> m_Shader;
    std::shared_ptr<CollisionShape> m_Collider;

    public:
    GameEntity(const char* _modelPath, const char* _texturePath, glm::vec3 _position);
    ~GameEntity();

    virtual void Update(float _delta);
    void Draw(Camera* _camera);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
    void SetID(int _id);
    void SetCollider(std::shared_ptr<CollisionShape> _shape);

    int GetID();
    Physics* GetPhysics() { return &m_Physics; };
    std::weak_ptr<CollisionShape> GetCollider();
};

#endif
