#pragma once

#include "IComponent.hpp"
#include "GameObject.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
class GameObject;

class ItemComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<IComponent>;

    ItemComponent(GameObject& gameObject, RigidBodyComponent& rigidBody) :  IComponent(gameObject), m_rigidBody(rigidBody)
    {
        m_spawnPosition = m_gameObject.getPosition();
    }

    bool init();
    void update(float deltaTime);

    virtual void SetInactive(); //resets the item and lifetime, positions it at the spawn position
    void SetCurrentlyUsed(bool used);
    virtual void SetThrown(bool thrown);
    bool isThrown();
    bool isCurrentlyUsed();

    void SetThrowForce(sf::Vector2f force); 

    GameObject& getGameObject() const
    {
        return m_gameObject;
    }

protected:
    float m_lifetime = 0.0f;
    float m_maxLifetime = 20.0f;    //items despawn after 20 seconds
    bool m_currentlyUsed = false;   //stops lifetime from counting down
    bool m_thrown = false;  //prevents thrown items from being picked up again
    sf::Vector2f m_spawnPosition;   
    RigidBodyComponent& m_rigidBody;
    sf::Vector2f m_throwForce;
};
} // namespace mmt_gd
