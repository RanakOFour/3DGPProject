#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <vector>

class VertexBuffer
{
    GLuint m_id;
    int m_components;
    bool m_dirty;
    std::vector<GLfloat> m_data;

public:
    VertexBuffer(int _componentsPerObject);
    VertexBuffer(const VertexBuffer& _copy);
    VertexBuffer& operator=(const VertexBuffer& _assign);
    ~VertexBuffer();

    void Add(glm::vec2 _value);
    void Add(glm::vec3 _value);
    void Add(glm::vec4 _value);

    int GetComponents();
    int GetDataSize();
    GLuint GetID();
};

#endif