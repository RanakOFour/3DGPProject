#ifndef CAMERA_H
#define CAMERA_H

#include "Game/GameEntity.h"
#include "GLPipeline/Skybox.h"
#include "GLPipeline/RenderTexture.h"
#include "Physics/Transform.h"

#include "SDL2/SDL.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <memory>

class Scene;
class Camera
{
    std::weak_ptr<Scene> m_Scene;
    Skybox m_Skybox;
    Transform m_Transform;
    glm::mat4 m_Projection;

    std::weak_ptr<GameEntity> m_Target;
    float m_DistanceFromTarget;
    float m_AngleAroundTarget;
    float m_Pitch;

    public:

    Camera();
    ~Camera();

    void SetScene(std::shared_ptr<Scene> _scene);
    void SetTarget(std::shared_ptr<GameEntity> _entity);

    void Update(float _delta);

    void DrawSkybox();
    void Use(ShaderProgram* _shader);
    void Translate(glm::vec3 _translation);
    void Rotate(glm::vec3 _eulerRotation);
    void SetPosition(glm::vec3 _pos);
    void SetRotation(glm::vec3 _euler);
    
    glm::vec3 GetPosition() { return m_Transform.GetPosition(); };
    std::weak_ptr<Scene> GetScene();
};

#endif