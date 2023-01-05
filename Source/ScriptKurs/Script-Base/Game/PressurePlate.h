#pragma once
#include <Game/Entity.h>

class Player;
struct GameUpdateContext;
class PressurePlate : public Entity
{
public:
	PressurePlate();
	const bool IsBeingPressed(Player* const aPlayer);
	InteractionResult Interact(const GameUpdateContext& context, Entity& interactingEntity) override;
};

