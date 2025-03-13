#include "Pipeline/VertexBuffer.h"
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <vector>
#include <stdio.h>

VertexBuffer::VertexBuffer(int _componentsPerObject) :
    m_id(0),
    m_components(_componentsPerObject),
    m_dirty(true),
    m_data()
{
    // Create buffer
    glGenBuffers(1, &m_id);

    if (!m_id)
    {
        printf("Failed to create VBO");
        throw std::exception();
    }

    printf("Created VBO with id %d\n", m_id);
}

VertexBuffer::~VertexBuffer()
{

}

void VertexBuffer::Add(glm::vec2 _value)
{
    m_data.push_back(_value.x);
    m_data.push_back(_value.y);
    m_dirty = true;

    printf("Added values %f %f to buffer %d\n", _value.x, _value.y, m_id);
}

void VertexBuffer::Add(glm::vec3 _value)
{
    m_data.push_back(_value.x);
    m_data.push_back(_value.y);
    m_data.push_back(_value.z);
    m_dirty = true;

    printf("Added values %f %f %f to buffer %d\n", _value.x, _value.y, _value.z, m_id);
}

void VertexBuffer::Add(glm::vec4 _value)
{
    m_data.push_back(_value.x);
    m_data.push_back(_value.y);
    m_data.push_back(_value.z);
    m_data.push_back(_value.w);
    m_dirty = true;

    printf("Added values %f %f %f %f to buffer %d\n", _value.x, _value.y, _value.z, _value.w, m_id);
}

int VertexBuffer::GetComponents()
{
    return m_components;
}

int VertexBuffer::GetDataSize()
{
    return sizeof(m_data[0]);
}

GLuint VertexBuffer::GetID()
{
    if (m_dirty)
    {
        if (!m_id)
        {
            glGenBuffers(1, &m_id);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_id);

        // Upload a copy of the data from memory into the new VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_data[0]) * m_data.size(), m_data.data(), GL_STATIC_DRAW);

        // Reset the state
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_dirty = false;

        printf("Vertex buffer %d cleaned\n", m_id);
    }

    return m_id;
}