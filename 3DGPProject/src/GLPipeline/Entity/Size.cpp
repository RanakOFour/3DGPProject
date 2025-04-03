#include "GLPipeline/Entity/Size.h"

#include <glm/ext.hpp>

Size::Size() :
	m_Dimensions(),
	m_Offset()
{

}

Size::Size(glm::vec3 _dimensions, glm::vec3 _offset) :
	m_Dimensions(_dimensions),
	m_Offset(_offset)
{

}

Size::~Size()
{

}