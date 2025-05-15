#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <iostream>
#include <memory>
#include <vector>

enum ShapeType
{
    Cube = 1,
    Sphere = 2,
    Mesh = 4
};

class Transform;
class GameEntity;
class CollisionShape
{
private:
    std::vector<bool> m_ContactList;
    std::vector<std::weak_ptr<CollisionShape>> m_Contacts;
    std::weak_ptr<GameEntity> m_Parent;
    std::weak_ptr<Transform> m_Transform;

    ShapeType m_Type;
    bool m_Environment;

public:
    CollisionShape(ShapeType _type, bool _env) : m_ContactList(),
                                                 m_Type(_type),
                                                 m_Environment(_env) 
                                                 {};

    CollisionShape(ShapeType _type, std::weak_ptr<GameEntity> _parent, bool _env) : m_ContactList(),
                                                                                    m_Type(_type),
                                                                                    m_Environment(_env),
                                                                                    m_Parent(_parent) {
                                                                                    };

    ~CollisionShape() {};

    virtual void OnCollisionEnter(std::weak_ptr<CollisionShape> _shape);
    virtual void OnCollisionStay(std::weak_ptr<CollisionShape> _shape);
    virtual void OnCollisionExit(std::weak_ptr<CollisionShape> _shape);

    void inline AddContact(std::weak_ptr<CollisionShape> _contact)
    {
        int L_index = FindContact(_contact);

        if (L_index == -1)
        {
            m_Contacts.push_back(_contact);
            m_ContactList.push_back(true);
            OnCollisionEnter(_contact);
        }
        else
        {
            m_ContactList[L_index] = true;
            OnCollisionStay(_contact);
        }
    };

    void inline ClearContacts()
    {
        for (int i = 0; i < m_ContactList.size(); ++i)
        {
            if (!m_ContactList[i])
            {
                OnCollisionExit(m_Contacts[i]);
                m_Contacts.erase(m_Contacts.begin() + i);
                m_ContactList.erase(m_ContactList.begin() + i);

                --i;
            }
            else
            {
                m_ContactList[i] = false;
            }
        }
    }

    int inline FindContact(std::weak_ptr<CollisionShape> _contact)
    {
        for (int i = 0; i < m_ContactList.size(); ++i)
        {
            if (m_Contacts[i].lock().get() == _contact.lock().get())
            {
                return i;
            }
        }

        return -1;
    }

    void SetParent(std::weak_ptr<GameEntity> _parent);
    std::weak_ptr<GameEntity> GetParent() { return m_Parent; };
    ShapeType GetType() { return m_Type; };
    bool Environment() { return m_Environment; };
    std::weak_ptr<Transform> GetTransform() { return m_Transform; };
};

#endif
