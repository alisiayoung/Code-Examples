#include "IComponent.hpp"
#include "ItemPool.hpp"

namespace mmt_gd
{
    class ItemSpawnerComponent : public IComponent
    {
    public:
        explicit ItemSpawnerComponent(GameObject& gameObject,
                                      size_t poolSize,
                                      sf::RenderWindow& renderWindow,
                                      const std::vector<std::string>& textureFiles,
                                      const std::vector<sf::IntRect>& textureRects,
                                      const std::vector<sf::FloatRect>& colliderRects,
                                      const std::vector<float>& itemMasses,
                                      std::vector<float>& spawnRates,
                                      const std::string& layerName);

        void spawnItem(sf::Vector2f directionSpeed);
        bool init() override;
        void update(float deltaTime) override;

    private:
        ItemPool m_itemPool;
        float m_timeSinceLastSpawn{};

        float m_spawnInterval = 1.f; // Spawn an item every second
        sf::RenderWindow* m_renderWindow;

        void prepareItem(GameObject& item); // Resets item to default state
    };
} // namespace mmt_gd
