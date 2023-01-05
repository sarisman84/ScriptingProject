#include "PressurePlate.h"

#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>
#include <Game/Player.h>



const bool PressurePlate::IsBeingPressed(Player* const /*aPlayer*/)
{
    return false;
}

PressurePlate::PressurePlate()
{
    
}

Entity::InteractionResult PressurePlate::Interact(const GameUpdateContext& /*context*/, Entity& /*interactingEntity*/)
{
    return InteractionResult::MoveOntoTile;
}
