#pragma once

#include <Game/Entity.h>

struct GameUpdateContext;
class Player : public Entity
{
	void Update(const GameUpdateContext& context) override;
};
