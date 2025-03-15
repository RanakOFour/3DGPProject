#include <memory>

#pragma once

class Model;
class Texture;
class Object
{
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Texture> m_Texture;

    public:
    Object(const char* _modelPath, const char* _texturePath);
    ~Object();

    void Draw();
};