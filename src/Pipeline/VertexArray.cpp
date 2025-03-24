#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include <GL/glew.h>
#include <vector>
#include <memory>
#include <string>

VertexArray::VertexArray() :
    m_id(0),
    m_dirty(true),
    m_bufferObjects()
{
    //Create array
    glGenVertexArrays(1, &m_id);

    if (!m_id)
    {
        printf("Failed to create VertexArray\n");
        throw std::exception();
    }

    printf("Created VAO id %d\n", m_id);
}

VertexArray::~VertexArray()
{

}

void VertexArray::SetBuffer(std::string _attribute, VertexBuffer* _buffer)
{
    //What is _attribute for?
    m_bufferObjects.push_back(_buffer);
    m_dirty = true;

    printf("Buffer id %d added to VAO id %d\n", m_id, _buffer->GetID());
}

int VertexArray::GetVertexCount()
{
    // Get size of vertex array
    return m_bufferObjects.size();
}

GLuint VertexArray::GetID()
{
    if (m_dirty)
    {
        if (!m_id)
        {
            glGenVertexArrays(1, &m_id);
        }


        glBindVertexArray(m_id);

        for (int i = 0; i < m_bufferObjects.size(); ++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[i]->GetID());

            glVertexAttribPointer(i, m_bufferObjects[i]->GetComponents(), GL_FLOAT,
                GL_FALSE, m_bufferObjects[i]->GetComponents() * m_bufferObjects[i]->GetDataSize(), (void*)0);

            printf("Buffer stride: %d\n", m_bufferObjects[i]->GetComponents() * m_bufferObjects[i]->GetDataSize());
            glEnableVertexAttribArray(i);
        }

        // Reset the state
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_dirty = false;
        printf("Vertex Array %d cleaned\n", m_id);
    }

    return m_id;
}
