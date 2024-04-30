#include "stdafx.h"

#include "ItemPool.hpp"
#include "CollisionCategories.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "RigidBodyComponent.hpp"
#include "RockComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "AnimationRenderComponent.hpp"

#include <iostream>
#include <memory>
#include "BombComponent.hpp"
#include <numeric>
#include <random>

mmt_gd::ItemPool::ItemPool(
    const size_t size,
    const std::string& layerName,
    sf::RenderWindow& renderWindow,
    const std::vector<sf::FloatRect>& colliderRects,
    const std::vector<std::string>& textureFiles,
    const std::vector<sf::IntRect>& textureRects,
    const std::vector<float>& itemDensities,
    std::vector<float>& spawnRates) :
    m_pool(size, nullptr)
{
    std::vector<float> cumulativeSpawnRates;
    std::partial_sum(spawnRates.begin(), spawnRates.end(), std::back_inserter(cumulativeSpawnRates));

    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (size_t i = 0; i < size; ++i)
    {
        float randNum = dis(gen);

        size_t itemType = std::distance(cumulativeSpawnRates.begin(),
                                        std::lower_bound(cumulativeSpawnRates.begin(), cumulativeSpawnRates.end(), randNum));

        auto& gameObject = m_pool[i];
        std::shared_ptr<RigidBodyComponent>    rigidBodyComponent;
        auto  fixtureDef = b2FixtureDef{};
        auto circleShape = b2CircleShape{};

        switch (itemType)
        {
            case 0:
            {
                gameObject = std::make_shared<GameObject>("Rock_" + std::to_string(m_globalItemIdx++));

                auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject, renderWindow, textureFiles[0], layerName);
                renderComp->getSprite().setTextureRect(textureRects[0]);
                renderComp->getSprite().setOrigin(textureRects[0].width / 2.F, textureRects[0].height / 2.F);

                rigidBodyComponent = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_dynamicBody);
                rigidBodyComponent->getB2Body()->SetEnabled(false);

                gameObject->addComponent<RockComponent>(*gameObject, *rigidBodyComponent);

                fixtureDef.density = itemDensities[0];

                circleShape.m_p.Set(0, 0);
                circleShape.m_radius = 0.5f;
                fixtureDef.shape     = &circleShape;
                break;
            }
            case 1:
            {
                gameObject = std::make_shared<GameObject>("Bomb_" + std::to_string(m_globalItemIdx++));

                auto animationComp = gameObject->addComponent<AnimationRenderComponent>(*gameObject,
                                                                                        renderWindow,
                                                                                        layerName,
                                                                                        textureFiles[1]);
                animationComp->addAnimation(AnimationType::IDLE,
                                            Direction::NONE,
                                            Animation::createAnimationFromLine(RowColl::Row,
                                                                               {textureRects[1].width, textureRects[1].height},
                                                                               {0, 0},
                                                                               4));
                animationComp->addAnimation(AnimationType::ATTACK,
                                            Direction::NONE,
                                            Animation::createAnimationFromLine(RowColl::Row,
                                                                               {textureRects[1].width, textureRects[1].height},
                                                                               {0, textureRects[1].height},
                                                                               8,
                                                                               {3, 3},
                                                                               true,
                                                                               0.1f));
                animationComp->setAnimation(AnimationType::IDLE, Direction::NONE);

                rigidBodyComponent = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_dynamicBody);
                rigidBodyComponent->getB2Body()->SetEnabled(false);

                gameObject->addComponent<BombComponent>(*gameObject, *rigidBodyComponent, 1000.f, 60.f);

                fixtureDef.density = itemDensities[1];

                circleShape.m_p.Set(0, 0);
                circleShape.m_radius = 0.5f;
                fixtureDef.shape     = &circleShape;

                rigidBodyComponent->getB2Body()->SetEnabled(false);

                break;
            }
        }

        fixtureDef.filter.categoryBits = static_cast<int>(CollisionCategories::ITEMS);
        fixtureDef.filter.maskBits     = 0xFFFF & ~static_cast<int>(CollisionCategories::KILLZONES) &
                                     ~static_cast<int>(CollisionCategories::COLLIDER) &
                                     ~static_cast<int>(CollisionCategories::ITEMS) &
                                     ~static_cast<int>(CollisionCategories::RIVER);

        gameObject->addComponent<ColliderComponent>(*gameObject, *rigidBodyComponent, fixtureDef);

        if (!gameObject->init())
        {
            sf::err() << "Could not initialize go " << gameObject->getId() << std::endl;
        }

        gameObject->setActive(false);
        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    }

    std::cout << "## pool-size: " << m_pool.size() << std::endl;
}

mmt_gd::GameObject::Ptr mmt_gd::ItemPool::get()
{
    return m_pool[m_counter++ % m_pool.size()];
}
