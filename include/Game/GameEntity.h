#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"

#include <glm/ext.hpp>
#include <memory>
#include <vector>

class Camera;
class Scene;
class GameEntity 
{
    // Loop back for "purposes"?
    std::weak_ptr<Scene> m_Scene;

    std::shared_ptr<Model> m_Model;

    // Could probably be accumulated into some other 'material' class
    std::shared_ptr<Texture> m_Texture;
    std::shared_ptr<ShaderProgram> m_Shader;

    // Properties
    int m_id;
    std::shared_ptr<Transform> m_Transform;
    std::shared_ptr<CollisionShape> m_Collider;

    bool m_Environment;

    public:
    GameEntity(glm::vec3 _position, float _size, bool _env);
    GameEntity(glm::vec3 _position, glm::vec3 _size, bool _env);
    GameEntity(std::shared_ptr<Model> _model, std::shared_ptr<Texture> _tex, glm::vec3 _position, glm::vec3 _size, bool _env);
    ~GameEntity();

    virtual void Update(float _delta);
    void Draw(Camera* _camera);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
    void SetID(int _id);
    int GetID();

    // Set an object so it won't move during collision resolution, could be done in CollisionShape?
    void FlagEnvironment() {m_Environment = true;};

    void SetScene(std::shared_ptr<Scene> _scene) { m_Scene = std::weak_ptr<Scene>(_scene); };
    glm::vec3 GetPosition() { return m_Transform->GetPosition(); };
    glm::vec3 GetRotation() { return m_Transform->EulerAngles(); };
    std::weak_ptr<CollisionShape> GetCollider() { return std::weak_ptr<CollisionShape>(m_Collider); };
};

#endif
