# Code-Examples

## Phantn't Item Spawner
The game Phantn't and the game engine was created by a colleague of mine and myself as part of a class in university.
With the code examples I want to showcase one of the parts that I made: A system for the throwable items that were spawned.

Since the game was component based, all Components derived from the IComponent class.

My general idea was to create an Item Component Clas that was inherited to the Rock Component andd the Bomb Component. The different items are managed through an Item Pool - that was holding a specific number of items - and an Item Spawner.

### RockComponent
I kept the Rock Component quite simple. All the rock does, is pushing a player. I relied on Box2D when it came to the physics between rocks and players.

### BombComponent
The Bomb Component is more complex in every way. Bombs are animated, they have a sound and there are multiple outcomes of collision between bombs and the players.
I register an OnCollision function when a BombComponent is initialized. 
The bomb isn't meant to push a player away like the rock does. Instead, it just landds wherever it collided with a player. After a short amount of time the bomb explodes and any player within proximity is yeeted away. The collision for this is handled in the handleExplosion function.
