#include "PressurePlate.h"

#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>
#include <Game/Player.h>




PressurePlate::PressurePlate()
{
    sprite = SpriteId::PlateUp;
}

Entity::InteractionResult PressurePlate::Interact(const GameUpdateContext& /*context*/, Entity& /*interactingEntity*/)
{
    return InteractionResult::MoveOntoTile;
}
