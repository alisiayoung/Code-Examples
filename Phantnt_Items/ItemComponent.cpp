#include "stdafx.h"
#include "ItemComponent.h"
#include "PhysicsManager.hpp"

bool mmt_gd::ItemComponent::init()
{
    return true;
}

void mmt_gd::ItemComponent::update(float deltaTime)
{
    if (m_gameObject.isActive() && !m_currentlyUsed)
    {
		m_lifetime += deltaTime;
        if (m_lifetime > m_maxLifetime)
        {
            setInactive();
		}
	}
}

void mmt_gd::ItemComponent::setInactive()
{   
    m_lifetime = 0.0f;
    m_gameObject.setActive(false);
    m_rigidBody.getB2Body()->SetLinearVelocity(b2Vec2_zero);
    PhysicsManager::setEnabled(*m_rigidBody.getB2Body(), false);
    m_rigidBody.setPosition(m_spawnPosition);
    m_gameObject.setPosition(m_spawnPosition);
}

void mmt_gd::ItemComponent::setCurrentlyUsed(bool used)
{
    m_currentlyUsed = used;
    if (used)
        m_lifetime = 0.0f;
}

void mmt_gd::ItemComponent::setThrown(bool thrown)
{
    m_thrown = thrown;
    if (m_thrown)
        m_currentlyUsed = false;
}

bool mmt_gd::ItemComponent::isThrown()
{
    return m_thrown;
}

bool mmt_gd::ItemComponent::isCurrentlyUsed()
{
    return m_currentlyUsed;
}

void mmt_gd::ItemComponent::setThrowForce(sf::Vector2f force)
{
    m_rigidBody.addVelocity(force);
    setThrown(true);
	m_throwForce = force;
}

