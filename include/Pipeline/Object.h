#include <memory>
#include <glm/ext.hpp>

#pragma once

class Model;
class Texture;
class ShaderProgram;
class Object
{
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Texture> m_Texture;
    glm::mat4 m_MatrixCoords;
    glm::vec3 m_Coords;

    public:
    Object(const char* _modelPath, const char* _texturePath);
    ~Object();

    void Draw(ShaderProgram* _shader);
    void Move(glm::vec3 _movement);
};