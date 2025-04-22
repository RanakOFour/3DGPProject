#include "GLPipeline/Entity/Collider/Collider.h"
#include "GLPipeline/Entity/Transform.h"

#include "glm/ext.hpp"

#include <memory>
#include <iostream>

Collider::Collider(Transform* _parentTransform,	ColliderType _type)
{
	m_Transform = std::shared_ptr<Transform>(_parentTransform);
	m_type = _type;
}

Collider::~Collider()
{

}

bool Collider::AxisTest(glm::vec3 axis, Face& _face, glm::vec3& boxHalfSize) 
{
	if (glm::length(axis) < 1e-6f){ return true; } // Skip near-zero axis

    glm::vec3 normAxis = glm::normalize(axis);

    printf("Axis test on tri positions:\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n",
           _face.a.position.x, _face.a.position.y, _face.a.position.z,
           _face.b.position.x, _face.b.position.y, _face.b.position.z,
           _face.c.position.x, _face.c.position.y, _face.c.position.z);

    float p0 = glm::dot(_face.a.position, normAxis);
    float p1 = glm::dot(_face.b.position, normAxis);
    float p2 = glm::dot(_face.c.position, normAxis);

    float minP = glm::min(p0, p1, p2);
    float maxP = glm::max(p0, p1, p2);

    float r = boxHalfSize.x * std::abs(normAxis.x) +
              boxHalfSize.y * std::abs(normAxis.y) +
              boxHalfSize.z * std::abs(normAxis.z);

    return !(minP > r || maxP < -r);
}

bool Collider::TriangleBoxIntersect(Face& _face, glm::vec3& _boxHalfSize) 
{
	glm::vec3 boxAxis[3] = { glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1) };

    glm::vec3 edges[3] = { _face.b.position - _face.a.position,
								 _face.c.position - _face.b.position,
								 _face.a.position - _face.c.position };

    // Test box normals
    for (int i = 0; i < 3; ++i)
	{
        if (!AxisTest(boxAxis[i], _face, _boxHalfSize)){ return false; }
    }

    // Test triangle normal
    glm::vec3 triNormal = glm::cross(edges[0], edges[1]);
    if (!AxisTest(triNormal, _face, _boxHalfSize)){ return false; }

    // Test edge cross axes
    for (int i = 0; i < 3; ++i) 
	{
        for (int j = 0; j < 3; ++j) 
		{
            glm::vec3 testAxis = glm::cross(edges[i], boxAxis[j]);
            if (!AxisTest(testAxis, _face, _boxHalfSize)){ return false; }
        }
    }

    return true;
}

bool Collider::IsColliding(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	printf("ERROR: Collider::IsColliding call!\n");
	return false;
}

void Collider::CollisionResponse(Transform* _transform, Collider* _otherCollider, Transform* _otherTransform)
{
	glm::vec3& L_pos = _transform->Position();
	glm::vec3& L_otherPos = _otherTransform->Position();

	float L_ammount = 0.1f;
	float L_step = 0.1f;

	if(_otherCollider->m_type == ColliderType::Box && m_type == ColliderType::Box)
	{
		while (true)
		{
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.x += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.x -= L_ammount;
			L_pos.x -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.x += L_ammount;
			L_pos.z += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.z -= L_ammount;
			L_pos.z -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.z += L_ammount;
			L_pos.y += L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.y -= L_ammount;
			L_pos.y -= L_ammount;
			printf("New PosA: %f, %f, %f\nNew PosB: %f, %f, %f\n", L_pos.x, L_pos.y, L_pos.z, L_otherPos.x, L_otherPos.y, L_otherPos.z);
			if (!IsColliding(_transform, _otherCollider, _otherTransform)){ break;}
			L_pos.y += L_ammount;
			L_ammount += L_step;
		}
	}
	else
	{
		if(IsColliding(_transform, _otherCollider, _otherTransform))
		{
			printf("Obj at %f, %f, %f is colliding with a mesh!\n", L_pos.x, L_pos.y, L_pos.z);
		}
	}
}

Transform* Collider::GetTransform()
{
	return m_Transform.get();
}

void Collider::PrintInfo()
{
	printf("ColliderInfo:\n	Transform Pos: %f, %f, %f\n	Collider Type: %d\n", m_Transform->Position().x, m_Transform->Position().y, m_Transform->Position().z, m_type);
}