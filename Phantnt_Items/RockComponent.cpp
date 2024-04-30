#include "stdafx.h"

#include "RockComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "RigidBodyComponent.hpp"

mmt_gd::RockComponent::RockComponent(GameObject& gameObject, 
                                     RigidBodyComponent& rigidBody) : 
                                     ItemComponent(gameObject, rigidBody) {}

bool mmt_gd::RockComponent::init()
{
    ItemComponent::init();
    return true;
}

void mmt_gd::RockComponent::update(float deltaTime)
{
    ItemComponent::update(deltaTime);
}
