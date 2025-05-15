#ifndef SKYBOX_H
#define SKYBOX_H


#include "GLPipeline/Model.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/ShaderProgram.h"

#include "glm/ext.hpp"

class Camera;
class Skybox
{
    private:
    Model m_Model;
    Texture m_Texture;
    ShaderProgram m_Program;

    glm::vec3 m_Size;

    public:
    Skybox(glm::vec3 _size);
    ~Skybox();
    
    void Draw(glm::mat4 _model, Camera* _camera);
};


#endif