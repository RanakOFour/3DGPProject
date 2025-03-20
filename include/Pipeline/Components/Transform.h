#include <glm/ext.hpp>

class Transform
{
    public:
    glm::vec3 m_position;
    glm::vec3 m_eulerRotation;

    Transform();
    ~Transform();
};