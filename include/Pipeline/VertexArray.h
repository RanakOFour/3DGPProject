#include <GL/glew.h>
#include <vector>
#include <memory>
#include <string>

#pragma once
class VertexBuffer;
class VertexArray
{
    GLuint m_id;
    bool m_dirty;
    std::vector<VertexBuffer*> m_bufferObjects;

public:
    VertexArray();
    VertexArray(const VertexArray& _copy);
    VertexArray& operator=(const VertexArray& _assigner);
    ~VertexArray();

    void SetBuffer(std::string _attribute, VertexBuffer* _buffer);
    int GetVertexCount();
    GLuint GetID();
};