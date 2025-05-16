#ifndef HUD_H
#define HUD_H

#include "GLPipeline/VertexArray.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"
#include <glm/ext.hpp>
#include <vector>
#include <memory>

class HUD
{
    struct HUDElement
    {
        std::shared_ptr<VertexArray> va;
        std::shared_ptr<Texture> texture;
        glm::vec2 position;
        glm::vec2 size;
    };

    std::shared_ptr<VertexBuffer> m_DefaultUVBuffer;
    std::unique_ptr<ShaderProgram> m_Shader;
    std::vector<std::shared_ptr<HUDElement>> m_Elements;
    glm::mat4 m_Projection;

public:
    HUD(glm::ivec2 _windowSize);
    ~HUD();

    void AddElement(const std::string &_texturePath, glm::vec2 _position, glm::vec2 _size);
    void Draw();
};

#endif