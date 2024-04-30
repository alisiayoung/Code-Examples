#pragma once

#include "ItemComponent.h"

namespace mmt_gd
{
class RockComponent final : public ItemComponent
{
public:
    RockComponent(GameObject& gameObject, RigidBodyComponent& rigidBody);

    bool init();
    void update(float deltaTime);
};
} // namespace mmt_gd
