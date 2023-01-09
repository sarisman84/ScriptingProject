#pragma once
#include <Game/Entity.h>

class Player;
struct GameUpdateContext;
class PressurePlate : public Entity
{
public:
	PressurePlate();
	InteractionResult Interact(const GameUpdateContext& context, Entity& interactingEntity) override;
};

