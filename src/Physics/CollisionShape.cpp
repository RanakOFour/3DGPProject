#include "Physics/Shape/CollisionShape.h"
#include "Game/GameEntity.h"


void CollisionShape::OnCollisionEnter(std::weak_ptr<CollisionShape> _shape)
{ 
    printf("OnCollisionEnter %d %d\n", m_Parent.lock()->GetID(), _shape.lock()->GetParent().lock()->GetID());
}

void CollisionShape::OnCollisionStay(std::weak_ptr<CollisionShape> _shape)
{
    printf("OnCollisionStay %d %d\n", m_Parent.lock()->GetID(), _shape.lock()->GetParent().lock()->GetID());
}

void CollisionShape::OnCollisionExit(std::weak_ptr<CollisionShape> _shape)
{ 
    printf("OnCollisionExit %d %d\n", m_Parent.lock()->GetID(), _shape.lock()->GetParent().lock()->GetID()); 
}

void CollisionShape::SetParent(std::weak_ptr<GameEntity> _parent)
{
    m_Parent = _parent;
    m_Transform = m_Parent.lock()->GetTransform();
}