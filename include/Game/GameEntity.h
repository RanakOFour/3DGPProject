#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"
#include "Physics/Transform.h"
#include "Physics/Shape/CollisionShape.h"
#include "Physics/Rigidbody.h"

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
    std::shared_ptr<Rigidbody> m_Rigidbody;

    float m_Mass;
    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;
    glm::vec3 m_AddForce;

    bool m_Environment;

    public:
    GameEntity(glm::vec3 _position, glm::vec3 _size, bool _env);
    GameEntity(std::shared_ptr<Model> _model, std::shared_ptr<Texture> _tex, glm::vec3 _position, glm::vec3 _size, bool _env);
    ~GameEntity();

    virtual void Update(float _delta);
    void Draw(Camera* _camera);
    void AddForce(glm::vec3 _force);
    void Move(glm::vec3 _movement);
    void Rotate(glm::vec3 _rotation);
    void Scale(glm::vec3 _scaleChange);
    void SetID(int _id);
    int GetID();

    // Set an object so it won't move during collision resolution, could be done in CollisionShape?
    void FlagEnvironment() {m_Environment = true;};

    void SetScene(std::shared_ptr<Scene> _scene);
    void SetShader(std::shared_ptr<ShaderProgram> _program) { m_Shader = _program; };
    glm::vec3 GetPosition() { return m_Transform->GetPosition(); };
    glm::vec3 GetRotation() { return m_Transform->EulerAngles(); };
    std::weak_ptr<Transform> GetTransform() { return std::weak_ptr<Transform>(m_Transform); };
    std::weak_ptr<CollisionShape> GetCollider() { return std::weak_ptr<CollisionShape>(m_Collider); };
    std::shared_ptr<Rigidbody> GetRigidbody() { return m_Rigidbody; };
};

#endif
