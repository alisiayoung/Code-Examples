#pragma once
#include "GameObject.hpp"
#include "ItemComponent.h"

#include <vector>

namespace mmt_gd
{
    class ItemPool
    {
    public:
        explicit ItemPool(size_t size,
                          const std::string& layerName,
                          sf::RenderWindow& renderWindow,
                          const std::vector<sf::FloatRect>& colliderRects,
                          const std::vector<std::string>& textureFiles,
                          const std::vector<sf::IntRect>& textureRects,
                          const std::vector<float>& itemMasses,
                          std::vector<float>& spawnRates);

        std::vector<GameObject::Ptr> m_pool;
        GameObject::Ptr get();

    private:
        size_t m_counter{};
        inline static int m_globalItemIdx = 0;
        int m_differentItems = 2;
    };
}; // namespace mmt_gd
