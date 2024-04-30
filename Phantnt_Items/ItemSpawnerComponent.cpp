#include "stdafx.h"

#include "ItemSpawnerComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "ColliderComponent.hpp"
#include <SFML/System.hpp>
#include <random>

mmt_gd::ItemSpawnerComponent::ItemSpawnerComponent(
    GameObject&        gameObject,
    size_t             poolSize,
    sf::RenderWindow&  renderWindow,
    const std::vector<std::string>& textureFiles,
    const std::vector<sf::IntRect>&       textureRects,
    const std::vector<sf::FloatRect>&     colliderRects,
    const std::vector<float>&             itemMasses,
    std::vector<float>&             spawnRates,
    const std::string&                    layerName) :
IComponent(gameObject),
m_renderWindow(&renderWindow),
m_itemPool(poolSize, layerName, renderWindow, colliderRects, textureFiles, textureRects, itemMasses, spawnRates)
{
}

void mmt_gd::ItemSpawnerComponent::update(float deltaTime)
{
    m_timeSinceLastSpawn += deltaTime;
    if (m_timeSinceLastSpawn >= m_spawnInterval)
    {
        m_timeSinceLastSpawn = 0.0f;
        spawnItem(sf::Vector2f(0.f, 100.f));
    }
}

void mmt_gd::ItemSpawnerComponent::prepareItem(GameObject& item)
{
	const auto& rb = item.getComponent<RigidBodyComponent>();
	auto cc = item.getComponent<ColliderComponent>();

	item.setActive(true);
	item.getComponent<ItemComponent>()->SetCurrentlyUsed(false);
	item.getComponent<ItemComponent>()->SetThrown(false);
	rb->getB2Body()->SetEnabled(true);
	rb->getB2Body()->SetLinearVelocity(b2Vec2_zero);
	rb->getB2Body()->SetAngularVelocity(0.0f);
	rb->setPosition(m_gameObject.getPosition());
	rb->getB2Body()->SetGravityScale(0.0f);
	cc->setSensor(true);
}

void mmt_gd::ItemSpawnerComponent::spawnItem(sf::Vector2f directionSpeed)
{

    const auto& item = m_itemPool.get();
    const auto  rb   = item->getComponent<RigidBodyComponent>();

    if (!item->isActive())
    {
        prepareItem(*item);

        std::random_device rd;
        std::mt19937                    gen(rd());
        std::uniform_int_distribution<> dis(-20, 20);

        int offset = dis(gen);

        sf::Vector2f position = m_gameObject.getPosition();
        position.x += offset;

        rb->setTransform(position, 0.0f);
        rb->addVelocity(directionSpeed);
    }
}

bool mmt_gd::ItemSpawnerComponent::init()
{
    return true;
}
