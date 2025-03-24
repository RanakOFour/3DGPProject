#include <glm/ext.hpp>

class Transform
{
    public:
    glm::vec3 m_position;
    glm::vec3 m_eulerRotation;
    glm::vec4 m_Rotation;

    glm::vec3 m_Forward;
    glm::vec3 m_Up;

    Transform();
    ~Transform();
};