#include "Game/HUD.h"
#include "GLPipeline/VertexBuffer.h"
#include "GLPipeline/Texture.h"

HUD::HUD(glm::ivec2 _windowSize) : 
m_Shader(std::make_unique<ShaderProgram>("./resources/shaders/hud/vert.vs", "./resources/shaders/hud/frag.fs")),
m_Projection(glm::ortho(0.0f, (float)_windowSize.x, (float)_windowSize.y, 0.0f))
{
    m_DefaultUVBuffer = std::make_shared<VertexBuffer>(2);
    m_DefaultUVBuffer->Add(glm::vec2(0.0f, 0.0f));
    m_DefaultUVBuffer->Add(glm::vec2(0.0f, 1.0f));
    m_DefaultUVBuffer->Add(glm::vec2(1.0f, 1.0f));
    m_DefaultUVBuffer->Add(glm::vec2(0.0f, 0.0f));
    m_DefaultUVBuffer->Add(glm::vec2(1.0f, 1.0f));
    m_DefaultUVBuffer->Add(glm::vec2(1.0f, 0.0f));
}

HUD::~HUD()
{
    // Automatic cleanup through smart pointers
}

void HUD::AddElement(const std::string &_texturePath, glm::vec2 _position, glm::vec2 _size)
{
    std::shared_ptr<HUDElement> L_newElement = std::make_shared<HUDElement>();
    L_newElement->texture = std::make_shared<Texture>(_texturePath);
    L_newElement->position = _position;
    L_newElement->size = _size;

    // Position buffer
    VertexBuffer* L_posBuffer = new VertexBuffer(2);
    L_posBuffer->Add(glm::vec2(_position.x, _position.y));
    L_posBuffer->Add(glm::vec2(_position.x, _position.y + _size.y));
    L_posBuffer->Add(glm::vec2(_position.x + _size.x, _position.y + _size.y));
    L_posBuffer->Add(glm::vec2(_position.x, _position.y));
    L_posBuffer->Add(glm::vec2(_position.x + _size.x, _position.y + _size.y));
    L_posBuffer->Add(glm::vec2(_position.x + _size.x, _position.y));

    // Vertex array
    L_newElement->va = std::make_shared<VertexArray>();
    L_newElement->va->SetBuffer("position", L_posBuffer);
    L_newElement->va->SetBuffer("texCoord", m_DefaultUVBuffer.get());

    m_Elements.push_back(std::shared_ptr<HUDElement>(L_newElement));
}

void HUD::Draw()
{
    m_Shader->Use();
    m_Shader->SetUniform("u_Projection", m_Projection);
    m_Shader->SetUniform("u_View", glm::mat4(1.0f));

    for (int i = 0; i < m_Elements.size(); ++i)
    {
        glBindVertexArray(m_Elements[i]->va->GetID());
        glBindTexture(GL_TEXTURE_2D, m_Elements[i]->texture->GetID());

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}