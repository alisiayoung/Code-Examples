#pragma once
#include "ItemComponent.h"


namespace mmt_gd
{
    class BombComponent final : public ItemComponent
    {
    public:
        BombComponent(GameObject& gameObject,
                      RigidBodyComponent& rigidBody,
                      float knockbackImpulse,
                      float explosionRadius);

        bool init();
        void update(float deltaTime);

        void setThrown(bool thrown) override;
        void setInactive() override;

        bool m_hasLanded = false;
        float m_knockbackImpulse;

    private:
        void landBomb();
        void explode(float deltaTime);
        void handleExplosion();

        sf::Vector2f m_throwPosition;
        float m_explosionRadius;
        float m_timeSinceLanded = 0.0f;
        float m_timeSinceExploded = 0.0f;
        float m_timeToExplode = 1.f;

        bool m_shouldExplode = false;
        sf::Vector2f m_knockbackDirection;

        sf::SoundBuffer m_buffer;
        sf::Sound m_explosionSound;
    };
} // namespace mmt_gd
