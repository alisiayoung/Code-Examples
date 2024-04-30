#include "stdafx.h"

#include "BombComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "PhysicsManager.hpp"
#include "CollisionCategories.hpp"
#include "VectorAlgebra2D.h"
#include "AnimationRenderComponent.hpp"

mmt_gd::BombComponent::BombComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, float knockbackImpulse, float explosionRadius) :
    ItemComponent(gameObject, rigidBody), 
    m_knockbackImpulse(knockbackImpulse), 
    m_explosionRadius(explosionRadius) {}

bool mmt_gd::BombComponent::init()
{
    // Initialize sound
    m_buffer.loadFromFile("../assets/Sounds/explosionCrunch_003.ogg");
    m_explosionSound.setBuffer(m_buffer);
    m_explosionSound.setVolume(75.0f);

    ItemComponent::init();

    auto collider = getGameObject().getComponent<ColliderComponent>();
    collider->registerOnCollisionFunction([this](ColliderComponent& a, ColliderComponent& b)
    {
        if (b.getBody().getB2Body()->GetFixtureList()->GetFilterData().categoryBits ==
            static_cast<int>(CollisionCategories::PLAYERS) && isThrown())
        {
            landBomb();
        }
    });

    return true;
}


void mmt_gd::BombComponent::update(float deltaTime)
{
	ItemComponent::update(deltaTime);

    if (isThrown())
    {
            if (m_throwForce.x > 0 && m_gameObject.getPosition().x > m_throwPosition.x + m_throwForce.x / 2)
            {
                landBomb();
			}
            if (m_throwForce.x < 0 && m_gameObject.getPosition().x < m_throwPosition.x + m_throwForce.x / 2)
            {
                landBomb();
            }
    }

    if (m_hasLanded)
    {
        m_gameObject.getComponent<ColliderComponent>()->getFixture()->SetSensor(true);
		m_timeSinceLanded += deltaTime;
        if (m_timeSinceLanded > m_timeToExplode)
        {
			explode(deltaTime); 
        }
    }
}

void mmt_gd::BombComponent::setThrown(bool thrown)
{
    ItemComponent::setThrown(thrown);
    m_throwPosition = m_gameObject.getPosition() + sf::Vector2f(0, 20);
}

void mmt_gd::BombComponent::setInactive()
{
	ItemComponent::setInactive();
	m_hasLanded = false;
	m_timeSinceLanded = 0.0f;
    m_timeSinceExploded = 0.0f;

    auto collider = m_gameObject.getComponent<ColliderComponent>();
    collider->getFixture()->SetSensor(true);
    collider->getFixture()->GetShape()->m_radius = 0.5f;
    m_gameObject.getComponent<AnimationRenderComponent>()->setAnimationType(AnimationType::IDLE);
}

void mmt_gd::BombComponent::landBomb()
{
    m_rigidBody.getB2Body()->SetLinearVelocity(b2Vec2_zero);
    m_rigidBody.getB2Body()->SetGravityScale(0.0f);
    m_hasLanded = true;
}

void mmt_gd::BombComponent::explode(float deltaTime)
{
    auto collider = m_gameObject.getComponent<ColliderComponent>();
    collider->getFixture()->GetShape()->m_radius = PhysicsManager::t2b(m_explosionRadius);
    m_gameObject.getComponent<AnimationRenderComponent>()->setAnimationType(AnimationType::ATTACK);

    if (m_explosionSound.getStatus() != sf::Sound::Playing)
    {
        m_explosionSound.play();
    }

    m_timeSinceExploded += deltaTime;

    if (m_timeSinceExploded > 0.8f)
    {
		SetInactive();
	}
    else if (m_timeSinceExploded > 0.1f) // delay before applying knockback
    {
		handleExplosion();
    }
}

void mmt_gd::BombComponent::handleExplosion()
{
    b2AABB aabb;
    b2Vec2 center   = m_rigidBody.getB2Body()->GetPosition();
    aabb.lowerBound = center - b2Vec2(PhysicsManager::s2b(m_explosionRadius), PhysicsManager::s2b(m_explosionRadius));
    aabb.upperBound = center + b2Vec2(PhysicsManager::s2b(m_explosionRadius), PhysicsManager::s2b(m_explosionRadius));

    // Callback class for querying all bodies within the explosion radius
    class QueryCallback : public b2QueryCallback
    {
    public:
        std::vector<b2Body*> foundBodies;

        bool ReportFixture(b2Fixture* fixture) override
        {
            foundBodies.push_back(fixture->GetBody());
            return true;
        }
    };

    QueryCallback queryCallback;
    m_rigidBody.getB2Body()->GetWorld()->QueryAABB(&queryCallback, aabb);

    // Apply knockback to all players within the explosion radius
    for (auto& body : queryCallback.foundBodies)
    {
        if (body->GetFixtureList()->GetFilterData().categoryBits & static_cast<int>(CollisionCategories::PLAYERS))
        {
            sf::Vector2f direction = MathUtil::unitVector((sf::Vector2f(PhysicsManager::b2s(body->GetPosition()))) - m_gameObject.getPosition());

            body->SetLinearVelocity(b2Vec2(PhysicsManager::s2b(direction.x * m_knockbackImpulse),
                                           PhysicsManager::s2b(direction.y * m_knockbackImpulse)));
        }
    }
}
