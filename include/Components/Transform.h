#include <glm/ext.hpp>

class Transform
{
    public:
    glm::vec3 m_Position;
    glm::vec3 m_EulerRotation;
    glm::vec4 m_Rotation;

    glm::vec3 m_Forward;
    glm::vec3 m_Up;

    Transform();
    ~Transform();


};