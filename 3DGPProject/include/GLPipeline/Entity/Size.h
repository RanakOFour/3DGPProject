#ifndef SIZE_H

#define SIZE_H

#include <glm/ext.hpp>

class Size
{
private:
	glm::vec3 m_Dimensions;
	glm::vec3 m_Offset;
public:
	Size();
	Size(glm::vec3 _dimensions, glm::vec3 _offset);
	~Size();

	inline glm::vec3 Offset()
	{
		return m_Offset;
	};
	
	inline float Height()
	{
		return m_Dimensions.y;
	};

	inline float Width()
	{
		return m_Dimensions.x;
	};

	inline float Depth()
	{
		return m_Dimensions.z;
	};
};

#endif